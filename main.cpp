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
        
        if (debugOn)
        {
            cout << "training data:" << endl;
            csvstream open_train(train_in);
            map<string, string> row;
            while (open_train >> row)
            {
                cout << "  " << "label = " << row["tag"] << ", content = " << row["content"] << endl;
            }

        }
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
        cout << "trained on " << num_posts << " examples" << endl;
        return posts_count;
    }

    // EFFECTS: Return a set of unique whitespace delimited words.x
    set<string> unique_words() {
        csvstream open_train(train_in);
        map<string, string> row;

        istringstream source(row["content"]);
        string word;
        while (open_train >> row) {
            istringstream source(row["content"]);
            string word;
            while (source >> word)
            {
                unique_word_set.insert(word);
            }
        }

        if (debugOn)
        {
            cout << "vocabulary size = " << unique_word_set.size() << endl;
        }
        return unique_word_set;
    }

    set<string> unique_labels() {
        string large_string = "";

        csvstream open_train(train_in);
        map<string, string> row;

        while (open_train >> row) {
            large_string += row["tag"];
            large_string += " ";
        }

        //cout << large_string << endl;
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

    map<pair<string, string>, int> train_posts_for_word_and_label()
    {

        map<string, string> row;
        for (auto label: unique_label_set)
        {
            for (auto word : unique_word_set)
            {
                int posts_appear_in = 0;
                csvstream open_train(train_in);
                //cout << word << endl;
                while (open_train >> row)
                {
                    if (label == row["tag"])
                    {
                        //cout << label << endl;
                        istringstream source(row["content"]);
                        //cout << source.str() << endl;
                        string w;
                        while (source >> w)
                        {
                            if (w == word)
                            {
                                posts_appear_in++;
                                break;
                            }
                        }
                        //source.clear();
                    } 
                }
                if (posts_appear_in != 0)
                {
                    posts_for_word_and_label.insert({{label, word}, posts_appear_in});
                }
                //posts_for_word_and_label.insert({{word, label}, posts_appear_in});
            }
    
        }
        return posts_for_word_and_label;
    }

    void compute_log_prior()
    {
        for (auto label: posts_for_label)
        {

            double lnPC = log(label.second)-log(num_posts);
            log_priors.insert({label.first, lnPC});

        }        
    }
    
    void compute_log_likelihood()
    {
        for (auto word: posts_for_word_and_label)
        {
            double lnPwC = log(word.second)-log(posts_for_label[word.first.first]);
            //cout<< lnPwC <<endl; 
            log_likelihoods.insert({{word.first.first, word.first.second}, lnPwC});
        }    
        //cout << log_likelihoods.size() << endl;
        //cout << log_likelihoods.first.begin()->first << log_likelihoods.begin()->second << endl;
    }

    void compute_log_probability(string test_in)
    {
        
       // map<string, string> row;
       // map<string, double> log_probs;
       
       cout << "test data:" << endl;
       csvstream open_test(test_in);
       map<string, string> row;
       pair <int, int> num_correct = {0, 0};
       while(open_test >> row)
       {
            pair<string, double> largest_log;
            for (auto label : log_priors)
            {
                //pair<string, double> log_prob;
                double current_log = 0.0;
                current_log += label.second;
                istringstream source(row["content"]);
                string w;
                while (source >> w)
                {
                    //cout << w << endl;
                     // check if word is in word for label training set
                    // increment log prob for label for post
                    pair<string, string> w_and_l(label.first, w);
                    if (log_likelihoods.count(w_and_l) == 1)
                    {
                        //cout<<"ran1" <<endl;
                        current_log += log_likelihoods[w_and_l];
                    }
                    else if (posts_for_word.find(w) != posts_for_word.end())
                    {
                        //cout<<"ran2" <<endl;
                        current_log += log(posts_for_word[w])-log(num_posts);
                    }
                    else
                    {
                        //cout<<"ran3" <<endl;
                        current_log += log(1)-log(num_posts);
                    }
                    //source.clear();
                }
                //cout << label.first << endl;
                //cout << current_log << endl;
                if (largest_log.first == "" && largest_log.second == 0)
                {
                    largest_log = {label.first, current_log};
                }
                else if (current_log > largest_log.second)
                {
                    largest_log = {label.first, current_log};
                }
                
            }
            //cout << largest_log.first << " " << largest_log.second << endl;


            istringstream source(row["content"]);
            cout << "  " << "correct = " << row["tag"] <<", predicted = " << largest_log.first << ", log-probability score = " << largest_log.second << endl;
            cout << "  " << "content = " << source.str() << endl << endl;
            if (row["tag"] == largest_log.first)
            {
                num_correct.first += 1;
            }
            num_correct.second += 1;
        }  
        cout << "  " << "performance: " << num_correct.first << " / " << num_correct.second << " posts predicted correctly" << endl;
    }

    void debug_Train()
    {
        if (debugOn)
        {
            cout << "classes:" << endl;
            for (auto label : log_priors)
            {
                cout << "  " << label.first << ", " << posts_for_label[label.first] << " examples, log_prior = " << label.second << endl;
            }

            cout << "classifier parameters:" << endl;
            for (auto WandL : log_likelihoods)
            {

                cout << "  " << WandL.first.first << ":" << WandL.first.second << ", count = " << posts_for_word_and_label[{WandL.first.first, WandL.first.second}] <<
                ", log-likelihood = " << WandL.second << endl;
            }
            cout << endl;
        }
    }







    /// testing
    void print_map()
    {
        for (auto i : posts_for_word_and_label){
            cout << i.first.first << ", " << i.first.second << " \t\t\t " << i.second << endl; 
        }
    }

    private:
        string train_in;
        bool debugOn;

        int num_posts;

        set<string> unique_word_set;
        set<string> unique_label_set;

        map<string, int> posts_for_word;
        map<string, int> posts_for_label;
        map<string, double> log_priors;
        map<pair<string, string>, double> log_likelihoods;
        map<pair<string, string>, int> posts_for_word_and_label;
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


   // cout << "Hello World!\n";

    Classifier test = Classifier(training, debug);
    test.train_num_posts();
    test.unique_words();
    cout << endl;

    test.unique_labels();
    test.train_posts_for_label();
    
    test.train_posts_for_word();
    //test.train_posts_for_word_and_label();
    //test.compute_log_likelihood();
    //test.compute_log_prior();

    //test.print_map();

    //test.debug_Train();

    //test.compute_log_probability(testing);
    return 0;
}


