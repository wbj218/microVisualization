    //
// Created by Yu Gan on 7/9/17.
//

#include <json.hpp>
#include <random>
#include <fstream>

using json = nlohmann::json;
using namespace std;

#define N_USERS 5
#define N_AVG_FOLLOWERS 2.0

int myrandom (int i) { return std::rand()%i;}


int main() {
    srand((unsigned int) time(NULL));

    default_random_engine generator((unsigned int) time(NULL));

    normal_distribution<double> distribution(N_AVG_FOLLOWERS, N_AVG_FOLLOWERS);
    json user_graph;
    vector<string> user_vec;
    for (int i = 0; i < N_USERS; i++) {
        user_vec.push_back("@user_" + to_string(i));
    }

    for (int i = 0; i < N_USERS; i++) {
        random_shuffle(user_vec.begin(), user_vec.end(), myrandom);
        double n_followers = distribution(generator);
        n_followers = n_followers < 1 ? 1 : n_followers;
        n_followers = n_followers > N_USERS - 1 ? N_USERS - 1 : n_followers;
        n_followers = round(n_followers);

        user_graph["@user_" + to_string(i)]["blocked_users"] = {};

        for (int j = 0; j < n_followers; j++){

            if (j == 0) {
                if (user_vec[0] != "@user_" + to_string(i))
                    user_graph["@user_" + to_string(i)]["followers"] = {user_vec[0]};
                else {
                    user_graph["@user_" + to_string(i)]["followers"] = {user_vec[1]};
                    j += 1;
                    n_followers += 1;
                }
            }

            else{
                if (user_vec[j] != "@user_" + to_string(i))
                    user_graph["@user_" + to_string(i)]["followers"]
                            .insert(user_graph["@user_" + to_string(i)]["followers"].end(),user_vec[j]);
                else {
                    j += 1;
                    n_followers += 1;
                    user_graph["@user_" + to_string(i)]["followers"]
                            .insert(user_graph["@user_" + to_string(i)]["followers"].end(),user_vec[j]);
                }

            }

        }

    }

    ofstream json_file;
    json_file.open("./user_graph.json");
    json_file << user_graph;
    json_file.close();
    


    
    return 0;


}
