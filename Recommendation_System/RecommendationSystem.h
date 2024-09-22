//
// Created on 2/20/2022.
//

#ifndef RECOMMENDATIONSYSTEM_H
#define RECOMMENDATIONSYSTEM_H

#include <iostream>
#include "User.h"
#include <map>
class MovieComp
{
public:
    /**
 * @brief Overloaded function call operator to compare two Movie objects
 * @param lhs The left-hand side Movie object
 * @param rhs The right-hand side Movie object
 * @return True if lhs is less than rhs, false otherwise
 */
    bool operator()(const sp_movie &lhs, const sp_movie &rhs) const
    {
        return *lhs < *rhs;
    }
};
/**
 * @brief Database structure for storing movie features
 */
typedef std::map<sp_movie , std::vector<double>, MovieComp> db_features;

/**
 * @brief Class representing a Recommendation System
 */
class RecommendationSystem
{
private:

    db_features database; // name of movies and their features
    /**
     * @brief Normalizes a vector
     * @param vec The vector to be normalized
     * @return The normalized vector
     */
    static double norm(const std::vector<double>& vec);
    /**
     * @brief Normalizes vectors in a rank_map
     * @param ranks The rank_map containing vectors to be normalized
     */
    static void norm_vec(rank_map& ranks);
    /**
      * @brief Calculates the dot product of two vectors
      * @param u The first vector
      * @param v The second vector
      * @return The dot product of u and v
      */
    static double dot_product(const std::vector<double>& u,
                              const std::vector<double>& v);
    /**
     * @brief Calculates the angle between two vectors
     * @param u The first vector
     * @param v The second vector
     * @return The angle between u and v, indicating their similarity
     */
    static double angle_between_vectors(const std::vector<double>& u,
                                        const std::vector<double>& v);

    /**
     * @brief Helper function to generate preference vector from rank_map
     * @param ranks The rank_map containing user ratings
     * @return The preference vector generated from ranks
     */
    std::vector<double> pref_vec_helper(rank_map &ranks);

    /**
     * @brief Helper function to recommend a movie based on user preferences
     * @param ranks The rank_map containing user ratings
     * @param pref_vec The preference vector generated from ranks
     * @return The smart pointer to the recommended movie
     */
    sp_movie recommended_helper(rank_map&ranks,
                                const std::vector<double>& pref_vec);
    /**
     * @brief Comparator function for sorting movies based on similarity score
     * @param a First pair of movie and similarity score
     * @param b Second pair of movie and similarity score
     * @return True if a's similarity score is greater than b's, false
     * otherwise
     */
    static bool cmp_movie(std::pair<sp_movie, double>& a,std::pair<sp_movie,
                   double>&b);
    /**
     * @brief Calculates similarity scores between a given movie and
     * other movies
     * @param ranks The rank_map containing user ratings
     * @param sim_map Map to store movie-similarity pairs
     * @param movie The movie for which similarity scores are calculated
     */
    void similarity_helper
            (const rank_map& ranks,std::map<sp_movie, double>&sim_map,
             const sp_movie &movie);
    /**
     * @brief Calculates predicted rating for a movie based on user
     * ratings and similarity scores
     * @param ranks The rank_map containing user ratings
     * @param sort_vec Vector containing pairs of movies and their
     * similarity scores
     * @param k Number of similar movies to consider
     * @return The predicted rating for the movie
     */
    static double prediction_helper
    (rank_map ranks,const std::vector<std::pair<sp_movie,
     double>>&sort_vec, int k);


public:
explicit RecommendationSystem() = default;
//    RecommendationSystem(const RecommendationSystem&) = delete;
//    RecommendationSystem& operator=(const RecommendationSystem&) = delete;

    /**
     * adds a new movie to the system
     * @param name name of movie
     * @param year year it was made
     * @param features features for movie
     * @return shared pointer for movie in system
     */
	sp_movie add_movie(const std::string& name,int year,
                       const std::vector<double>& features);


    /**
     * a function that calculates the movie with highest score based on movie
     * features
     * @param ranks user ranking to use for algorithm
     * @return shared pointer to movie in system
     */
	sp_movie recommend_by_content(const User& user);

    /**
     * a function that calculates the movie with highest predicted score based
     * on ranking of other movies
     * @param ranks user ranking to use for algorithm
     * @param k
     * @return shared pointer to movie in system
     */
	sp_movie recommend_by_cf(const User& user, int k);


    /**
     * Predict a user rating for a movie given argument using item cf
     * procedure with k most similar movies.
     * @param user_rankings: ranking to use
     * @param movie: movie to predict
     * @param k:
     * @return score based on algorithm as described in pdf
     */
    double predict_movie_score(const User &user, const sp_movie &movie,
												  int k);

	/**
	 * gets a shared pointer to movie in system
	 * @param name name of movie
	 * @param year year movie was made
	 * @return shared pointer to movie in system
	 */
	sp_movie get_movie(const std::string &name, int year) const;

    bool operator<(const RecommendationSystem&rhs) const;

    friend std::ostream &operator<<(std::ostream &os,
                                    const RecommendationSystem
                                    &recommendationSystem);
};


#endif //RECOMMENDATIONSYSTEM_H
