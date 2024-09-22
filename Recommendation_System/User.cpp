

// don't change those include
#include "User.h"

//#include <utility>
#include "RecommendationSystem.h"


// implement your cpp code here
User::User(const std::string& user_name, const rank_map& users_rank,
           std::shared_ptr<RecommendationSystem>& rs)
{
   username =  user_name;
   user_rank = users_rank;
   recommend_system = rs;
}

const std::string& User::get_name() const
{
    return username;
}

void User::add_movie_to_rs(const std::string &name, int year,
                           const std::vector<double> &features, double rate)
{
    sp_movie new_sp_movie = recommend_system->add_movie(name, year, features);
    user_rank[new_sp_movie] = rate; // O(1) average
}

const rank_map& User::get_ranks() const
{
    return user_rank;
}

sp_movie User::get_recommendation_by_content() const
{
    return recommend_system->recommend_by_content(*this);
}

sp_movie User::get_recommendation_by_cf(int k) const
{
    return recommend_system->recommend_by_cf(*this, k);
}

double User::get_prediction_score_for_movie(const std::string &name, int year,
                                            int k) const
{
    sp_movie new_movie = recommend_system->get_movie(name, year);
    return recommend_system->predict_movie_score(*this, new_movie,
                                                 k);
}


std::ostream &operator<<(std::ostream &os, const User &user)
{
    os<<"users_name: "<<user.get_name()<< "\n"<<*(user.recommend_system)
    <<std::endl;
    return os;
}