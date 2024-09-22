#include "RecommendationSystem.h"

sp_movie RecommendationSystem::get_movie(const std::string &name, int year)
const
{
    sp_movie desired_sp = std::make_shared<Movie>(name, year);
    auto it = database.find(desired_sp);
    if (it != database.end())
    {
        return it->first;
    }
    return nullptr;
}

sp_movie RecommendationSystem::add_movie(const std::string &name, int year,
                                         const std::vector<double> &features)
{
    sp_movie new_movie = std::make_shared<Movie>(name, year);
    database[new_movie] = features;
    return new_movie;
}


double RecommendationSystem::dot_product(const std::vector<double> &u,
                         const std::vector<double> &v)
{
    double product = 0;
    for (size_t i = 0; i < u.size(); ++i) {
        product += u[i] * v[i];
    }
    return product;
}

double RecommendationSystem::norm(const std::vector<double>& vec) {
    double result = 0;
    for (const auto &val : vec) {
        result += pow(val, 2);
    }
    return sqrt(result);
}

double RecommendationSystem::angle_between_vectors
        (const std::vector<double>& u,const std::vector<double>& v)
{
    return (dot_product(u, v) / (norm(u) * norm(v)));
}

void RecommendationSystem:: norm_vec(rank_map& ranks)
{
    double average, sum = 0, count = 0;
    for (auto & it : ranks)
    {
        if (it.second != 0)
        {
            sum += it.second;
            count++;
        }
    }
    average = sum / count;
    for (auto& it: ranks)
    {
        if (it.second != 0)
        {
            it.second-=average;
        }
    }
}


std::vector<double> RecommendationSystem::pref_vec_helper
        (rank_map &ranks)
{
    double rank;
    sp_movie movie;
    std::vector<double> tmp;
    size_t vec_size = database.begin()->second.size();
    std::vector<double> vec_features(vec_size, 0.0);
    for (const auto& elem: ranks)
    {
        movie = elem.first;
        rank = elem.second;
        if (rank != 0)
        {
            tmp = database[movie];
            for (size_t i = 0; i < vec_size; ++i)
            {
                vec_features[i]+= (tmp[i] * rank);
            }
        }
    }
    return vec_features;
}

sp_movie RecommendationSystem::recommended_helper
        (rank_map &ranks,const std::vector<double> &vec_pref)
{
    double max = -2, cur = 0;
    sp_movie res_movie;
    for (const auto& elem : database)
    {

        if ((ranks.count(elem.first) == 0) ||
        (ranks.count(elem.first) == 1 &&
        (ranks.find(elem.first)->second == 0)))
        {
            cur=angle_between_vectors(vec_pref,elem.second);
            if (max <cur)
            {
                res_movie= elem.first;
                max= cur;
            }
        }
    }
    return res_movie;
}


sp_movie RecommendationSystem::recommend_by_content(const User &user)
{
    rank_map ranks = user.get_ranks();
    norm_vec(ranks);
    std::vector<double> vec_features = pref_vec_helper(ranks);
    return recommended_helper(ranks, vec_features);
}

sp_movie RecommendationSystem::recommend_by_cf(const User &user, int k)
{
    double max = -2, cur = 0;
    sp_movie curr_movie, result;
    rank_map ranks =user.get_ranks();
    for (auto &elem :database)
    {
        if ((ranks.count(elem.first) == 0) ||
        (ranks.count(elem.first) == 1 &&
        (ranks.find(elem.first)->second == 0)))
        {
            cur = predict_movie_score(user, elem.first, k);
            if (cur > max)
            {
                result = elem.first;
                max = cur;
            }
        }
    }
    return result;
}

bool RecommendationSystem::cmp_movie(std::pair<sp_movie,
                                     double>& m1,std::pair<sp_movie,
                                     double>&m2)
{
    return m1.second > m2.second;
}


void RecommendationSystem::similarity_helper(const rank_map& ranks,
                                    std::map<sp_movie, double>&similarities,
                                    const sp_movie&movie)
 {
    double angle;
    for (const auto&elem :ranks)
    {
        sp_movie movie_ptr= elem.first;
        double movie_rank =elem.second;
        if (movie_rank != 0)
        {
            angle = angle_between_vectors(database[movie],
                                       database[movie_ptr]);
            similarities[movie_ptr] = angle;
        }
    }
 }
 double RecommendationSystem::prediction_helper
        (rank_map ranks,const std::vector<std::pair<sp_movie,
                double>>&sort_vec, int k)
 {
    double calc = 0, sum = 0;
    for (const auto& elem: sort_vec)
     {
      if (!k)
      {
          break;
      }
      calc+=elem.second * ranks[elem.first];
      sum+=elem.second;
      --k;
     }
     return calc/sum;
 }

double RecommendationSystem::predict_movie_score(const User &user,
                                                 const sp_movie &movie, int k)
{
    std::map<sp_movie, double> similarities;
    const rank_map& ranks = user.get_ranks();
    this->similarity_helper(ranks,similarities, movie);
    std::vector<std::pair<sp_movie, double>>sorting(similarities.begin(),
                                                     similarities.end());
    std::sort(sorting.begin(),sorting.end(), cmp_movie);
    return prediction_helper(ranks,sorting, k);
}

bool RecommendationSystem:: operator<(const RecommendationSystem&rhs) const
{
    return database < rhs.database;
}


std::ostream &operator<<(std::ostream &os, const RecommendationSystem
&recommendationSystem)
{
    for (const auto & db_feature :recommendationSystem.database)
    {
        os<<*(db_feature.first)<< " ";
    }
    return os;
}





