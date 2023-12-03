#include <iostream>
#include "csvstream.hpp"
#include <set>
#include <cmath>

using namespace std;

class Classifier {
    public:
    Classifier(string filein, bool do_debug)
    {
        train_in = filein;
        debugOn = do_debug;
        
    }
    



    int train_num_posts()
    {
        int posts_count = 0;
        csvstream open_train(train_in);
        map<string, string> row;
        while (open_train >> row) {
            ++posts_count;
        }

        num_posts = posts_count;
        return posts_count;
    }

    // EFFECTS: Return a set of unique whitespace delimited words.x
    set<string> unique_words() {
        string large_string = "";

        csvstream open_train(train_in);
        map<string, string> row;

        while (open_train >> row) {
            large_string += row["content"];
            large_string += " ";
        }

        cout << large_string << endl;
        istringstream source(large_string);
        set<string> words;
        string word;
        while (source >> word) {
            words.insert(word);
        }
        unique_word_set = words;
        return words;
    }

    set<string> unique_labels() {
        string large_string = "";

        csvstream open_train(train_in);
        map<string, string> row;

        while (open_train >> row) {
            large_string += row["tag"];
            large_string += " ";
        }

        cout << large_string << endl;
        istringstream source(large_string);
        set<string> labels;
        string label;
        while (source >> label) {
            labels.insert(label);
        }
        unique_label_set = labels;
        return labels;
    }

    int train_unique_words()
    {
        return unique_word_set.size();
    }

    map<string, int>  train_posts_for_word()
    {
        //set<string>::iterator w;
        map<string, string> row;

        
        for (auto w : unique_word_set) 
        {
            csvstream open_train(train_in);
            int posts_appear_in = 0;
            while (open_train >> row) 
            {
                istringstream source(row["content"]);
                string word;
                while (source >> word) 
                {
                    if (w == word)
                    {
                        posts_appear_in++;
                        break;
                        
                    }
                    //cout << "loop inner works" << endl;
                    source.clear();
                }
                //cout << "loop outer works" << endl;
                
            }
            posts_for_word.insert({w, posts_appear_in});
        }
        
        return posts_for_word;
    }

    map<string, int> train_posts_for_label()
    {
        //set<string>::iterator C;
        
        map<string, string> row;
        for (auto l : unique_label_set)
        {
            int posts_appear_in = 0;
            csvstream open_train(train_in);
            while (open_train >> row) 
            {
                istringstream source(row["tag"]);
                string label;
                while (source >> label) 
                {
                    if (l == label)
                    {
                        posts_appear_in++;
                        break;
                    }
                    source.clear();
                }
            }
            posts_for_label.insert({l, posts_appear_in});
        }
        
        return posts_for_label;
    }

    map<string, map<string, int>> train_posts_for_word_and_label()
    {
  
        //inner map
        map<string, int> label_and_int;
        
        //open file
        csvstream open_train(train_in);
        map<string, string> row;
        //add labels to map
        for (auto word: unique_word_set)
        {
            int posts_appear_in = 0;
            for (auto label: unique_label_set)
            {
                while (open_train >> row) 
                {
                    istringstream source(row["tag"]);
                    string l;
                    if (l == label)
                    {
                        istringstream source(row["content"]);
                        string w;
                         while (source >> word) 
                         {
                            if (w == word)
                            {
                                posts_appear_in++;
                                break;
                            }
                         }

                    }
                }
                label_and_int.insert({label, posts_appear_in});
                posts_for_word_and_label.insert({word, label_and_int});
            }
        }
        return posts_for_word_and_label;
    }

    void compute_log_prior()
    {
        for (auto label: posts_for_label)
        {

            double lnPC = log(label.second/num_posts);
            log_priors.insert({label.first, lnPC});

        }        
    }
    
    void compute_log_likelihood()
    {
        for (auto word: posts_for_word_and_label)
        {
            double lnPwC = log((word.second.begin()->second)/num_posts);
            cout<< lnPwC<<endl; 
            log_likelihoods.insert({{word.first, word.second.begin()->first}, lnPwC});
        }    
        cout << log_likelihoods.size() << endl;
        //cout << log_likelihoods.first.begin()->first << log_likelihoods.begin()->second << endl;
    }

    pair<string, double> compute_log_probability(string test_in)
    {
        
        map<string, string> row;
        map<string, double> log_probs;
       
        // iterate over labels
        for (auto label : log_priors)
        {
            csvstream open_test(test_in);

            double current_log = 0.0;
            current_log += log_priors[label.first];
             // iterate over posts
             while (open_test >> row) 
            {
                istringstream source(row["content"]);
                string word;
                // iterate over words in posts
                while (source >> word) 
                {
                    // check if word is in word for label training set
                    // increment log prob for label for post
                    pair<string, string> w_and_l(word, label.first);
                    if (log_likelihoods.count(w_and_l) == 1)
                    {
                        current_log += log_likelihoods[w_and_l];
                    }
                    else if (unique_word_set.count(word) == 1)
                    {
                        current_log += log(posts_for_word[word]/num_posts);
                    }
                    else
                    {
                        current_log += 1/num_posts;
                    }
                    source.clear();
                }
                
            }
            log_probs.insert({label.first, current_log});
        }
        
        pair<string, double> largest_log;
        map<string, double>::iterator it = log_probs.begin();
        //find largest value
        // NEED TO CHECK FOR EQUALS
        for (auto label : log_probs)
        {
            if (it == log_probs.begin())
            {
                largest_log.first = it->first;
                largest_log.second = it->second;
            }

            it++;
            if (it->second > largest_log.second)
            {
                largest_log.first = it->first;
                largest_log.second = it->second;
            }
            else if (it == log_probs.end())
            {
                break;
            }

            
        }

        return largest_log;
    }








































    /// testing
    void print_map()
    {
        for (auto i : posts_for_label){
            cout << i.first << " \t\t\t " << i.second << endl; 
        }
    }

    private:
        string train_in;
        bool debugOn;

        int num_posts;
        //int unique_words;

        set<string> unique_word_set;
        set<string> unique_label_set;

        map<string, int> posts_for_word;
        map<string, int> posts_for_label;
        map<string, double> log_priors;
        map<pair<string, string>, double> log_likelihoods;
        map<string, map<string, int>> posts_for_word_and_label;
};




int main(int argc, char *argv[]) {

    cout.precision(3);

    string training;
    string testing;
    bool debug = false;

    if (argc > 4 || argc < 3)
    {
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return 1;
    }

    if (argv[3] != NULL)
    {
        debug = true;
    }

    training = argv[1];
    testing = argv[2];

    try {
        csvstream open_training(training);
    } catch(const csvstream_exception &e) {
        cout << "Error opening file: " << training << endl;
        return 1;
    }

    try {
        csvstream open_testing(testing);
    } catch(const csvstream_exception &e) {
        cout << "Error opening file: " << testing << endl;
        return 1;
    }


    cout << "Hello World!\n";

    Classifier test = Classifier(training, debug);
    cout << test.train_num_posts() << endl;
    test.unique_words();
    cout << test.train_unique_words() << endl;
    test.unique_labels();
    cout << test.unique_labels().size() << endl;

    test.train_posts_for_word();
    test.train_posts_for_label();
    test.print_map();

    test.compute_log_likelihood();
    test.compute_log_prior();
    cout << test.compute_log_probability(testing).first << endl;
    cout << test.compute_log_probability(testing).second << endl;


    return 0;
}


