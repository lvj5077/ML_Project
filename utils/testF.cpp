/**
 * Date:  2016
 * Author: Rafael Muñoz Salinas
 * Description: demo application of DBoW3
 * License: see the LICENSE.txt file
 */
#include "myImage.h"

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

// DBoW3
#include "DBoW3.h"

// OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#ifdef USE_CONTRIB
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/xfeatures2d.hpp>
#endif
#include "DescManip.h"


#include <opencv2/opencv.hpp>
#include <fstream>

#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>



#include <string>
#include <iostream>
#include <boost/filesystem.hpp>
#include <opencv2/imgproc.hpp>

using namespace DBoW3;
using namespace std;
using namespace cv;
using namespace boost::filesystem;



//command line parser
class CmdLineParser{int argc; char **argv; public: CmdLineParser(int _argc,char **_argv):argc(_argc),argv(_argv){}  bool operator[] ( string param ) {int idx=-1;  for ( int i=0; i<argc && idx==-1; i++ ) if ( string ( argv[i] ) ==param ) idx=i;    return ( idx!=-1 ) ;    } string operator()(string param,string defvalue="-1"){int idx=-1;    for ( int i=0; i<argc && idx==-1; i++ ) if ( string ( argv[i] ) ==param ) idx=i; if ( idx==-1 ) return defvalue;   else  return ( argv[  idx+1] ); }};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

// extended surf gives 128-dimensional vectors
const bool EXTENDED_SURF = false;
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

void wait()
{
    cout << endl << "Press enter to continue" << endl;
    getchar();
}

vector< cv::Mat  >  loadFeatures( std::vector<string> path_to_images,string descriptor="") throw (std::exception){
    //select detector
    cv::Ptr<cv::Feature2D> fdetector;
    if (descriptor=="orb")        fdetector=cv::ORB::create();
    else if (descriptor=="brisk") fdetector=cv::BRISK::create();
#ifdef OPENCV_VERSION_3
    else if (descriptor=="akaze") fdetector=cv::AKAZE::create();
#endif
#ifdef USE_CONTRIB
    else if(descriptor=="surf" )  fdetector=cv::xfeatures2d::SURF::create(400, 4, 2, EXTENDED_SURF);
#endif

    else throw std::runtime_error("Invalid descriptor");
    assert(!descriptor.empty());
    vector<cv::Mat>  features;


    cout << "Extracting   features..." << endl;
    for(size_t i = 0; i < path_to_images.size(); ++i)
    {
        vector<cv::KeyPoint> keypoints;
        cv::Mat descriptors;
        // cout<<"reading image: "<<path_to_images[i]<<endl;
        cv::Mat image = cv::imread(path_to_images[i], 0);
        if(image.empty())throw std::runtime_error("Could not open image"+path_to_images[i]);
        // cout<<"extracting features"<<endl;
        fdetector->detectAndCompute(image, cv::Mat(), keypoints, descriptors);
        features.push_back(descriptors);
        // cout<<"done detecting features"<<endl;
    }
    return features;
}

// ----------------------------------------------------------------------------

void testVocCreation(const vector<cv::Mat> &features)
{
    // branching factor and depth levels
    const int k = 9;
    const int L = 3;
    const WeightingType weight = TF_IDF;
    const ScoringType score = L1_NORM;

    DBoW3::Vocabulary voc(k, L, weight, score);

    cout << "Creating a small " << k << "^" << L << " vocabulary..." << endl;
    voc.create(features);
    cout << "... done!" << endl;

    cout << "Vocabulary information: " << endl
         << voc << endl << endl;

    // lets do something with this vocabulary
    cout << "Matching images against themselves (0 low, 1 high): " << endl;
    BowVector v1, v2;
    for(size_t i = 0; i < features.size(); i++)
    {
        voc.transform(features[i], v1);
        for(size_t j = 0; j < features.size(); j++)
        {
            voc.transform(features[j], v2);

            double score = voc.score(v1, v2);
            // cout << "Image " << i << " vs Image " << j << ": " << score << endl;
        }
    }

    // save the vocabulary to disk
    cout << endl << "Saving vocabulary..." << endl;
    voc.save("/Users/Q/Desktop/ML_Project/QueryResults/small_voc.yml.gz");
    cout << "Vocabulary saved! " << endl<<endl;
}

////// ----------------------------------------------------------------------------

void creatDatabase(const  vector<cv::Mat > &features)
{
    cout << "Creating a small database..." << endl;

    // load the vocabulary from disk
    Vocabulary voc("/Users/Q/Desktop/ML_Project/QueryResults/small_voc.yml.gz");

    Database db(voc,true,0);
    //Database db(voc, false, 0); // false = do not use direct index
    // (so ignore the last param)
    // The direct index is useful if we want to retrieve the features that
    // belong to some vocabulary node.
    // db creates a copy of the vocabulary, we may get rid of "voc" now

    // add images to the database
    for(size_t i = 0; i < features.size(); i++){
        db.add(features[i]);
    }
    cout << "Feature loaded!" << endl;

    cout << "Database information: " << endl << db << endl;
    db.save("/Users/Q/Desktop/ML_Project/QueryResults/small_db.yml.gz");
    cout << "Database save!" << endl << endl;
}

void queryDatabase(const  vector<cv::Mat> &features_query, myImage image[] ,vector<string> images_query) {
    cout << "Retrieving database ..." << endl;
    Database db("/Users/Q/Desktop/ML_Project/QueryResults/small_db.yml.gz");
    cout << "Database loaded! This is: " << endl << db << endl;


    cout << "Querying the database: " << endl;

    QueryResults ret;
    for(size_t i = 0; i < features_query.size(); i++)
    {  

    	cout << "---------------------------------- "<< endl;
        db.query(features_query[i], ret, 2);

        cout << i <<endl;
        cout << "ret id: "<<ret[1].Id << endl;
        cout << "ret score: "<<ret[1].Score << endl;
    	cout << "---------------------------------- "<< endl;
        // ret[0] is always the same image in this case, because we added it to the
        // database. ret[1] is the second best match.

        image[ret[1].Id].getLabel();

        Mat imageM;
        imageM = imread(images_query[i],0);     
        string resultLabel= image[ret[1].Id].getLabel();  
        putText( imageM, resultLabel, Point( imageM.rows/3,imageM.cols/7),CV_FONT_HERSHEY_COMPLEX, 2, Scalar::all(255), 3, 8 );  
        imshow("Result Window",imageM); 
        cv::waitKey(0);
        
        cout << "Searching for Image " << i << endl;
    }

    cout << endl;
}

// ----------------------------------------------------------------------------

void selfEvaluate(const  vector<cv::Mat> &features, myImage image[]) {
    cout << "Retrieving database ..." << endl;
    Database db("/Users/Q/Desktop/ML_Project/QueryResults/small_db.yml.gz");
    cout << "Database loaded! This is: " << endl << db << endl;


    cout << "Querying the database: " << endl;

    QueryResults ret;
    int resultN = 0;
    for(size_t i = 0; i < features.size(); i++)
    {  
    	// cout << "---------------------------------- "<< endl;
     	// db.query(features[i], ret, 2);
     	// cout << "ret id: "<<ret[1].Id << endl;
     	// cout << "ret score: "<<ret[1].Score << endl;
    	// cout << "---------------------------------- "<< endl;
        // ret[0] is always the same image in this case, because we added it to the
        // database. ret[1] is the second best match.

    	if (image[ret[1].Id].getLabel() == image[i].getLabel()){
    		resultN = resultN + 1;
    	}
    	else{
    		cout << "Mismatch!"<<endl;
    	}

    }

    cout << endl << "Accuracy: " << resultN/features.size() <<endl <<endl;
    // cout << endl;
}

// ----------------------------------------------------------------------------

vector<string> readImageDirs(string inDir){
    path inPath(inDir);
    vector<string> paths;

    for (auto i = directory_iterator(inPath); i != directory_iterator(); i++)
    {
        if (is_directory(i->path()) ) //we eliminate directories
        {
            paths.push_back(inDir+"/"+i->path().filename().string());
        }
        else
            continue;
    }

    return paths;
}

// ----------------------------------------------------------------------------

vector<string> readImageNames(string inDir){
    path inPath(inDir);
    vector<string> paths;

    for (auto i = directory_iterator(inPath); i != directory_iterator(); i++)
    {
        if (!is_directory(i->path()) ) //we eliminate directories
        {
            paths.push_back(inDir+"/"+i->path().filename().string());
        }
        else
            continue;
    }

    return paths;
}

// ----------------------------------------------------------------------------


int main (int argc, char *argv[]) {

    if (argc != 4) {
        cerr << "wrong number of arguments"<<endl;
        cout << "usage: ./train ./test" << endl;
    }

    string descriptor=argv[1];

    string train_dir = argv[2];
    int trainDirLen = train_dir.length();
    cout<<"train_dir: "<<train_dir<<endl;
    string test_dir = argv[3];
    cout<<"test_dir: "<<test_dir<<endl;

    vector<string> images_query = readImageNames(test_dir);

    vector<string> train_dirNames = readImageDirs (train_dir);

    vector<string> labelNames;
    vector<string> images_train;

    cout << train_dirNames.size() << " Sence Topics" << endl;

    for(int i = 0; i < train_dirNames.size() ; i++){
        cout << train_dirNames[i] << endl;
        labelNames.push_back(train_dirNames[i].substr(trainDirLen+1));
    }
    
    cout << "---------------------------------- "<< endl;
    for(int i = 0; i < train_dirNames.size() ; i++)
        cout << "label "<<(i+1)<<": "<<labelNames[i] << endl;

    cout << "---------------------------------- "<< endl;
    string imageDir;
    vector<int> topicSize;
    for(int i = 0; i < train_dirNames.size(); i++){
        cout << "Under "<<train_dirNames[i] << endl;

        imageDir = train_dirNames[i];
        vector<string>  imageNames = readImageNames(imageDir);
        topicSize.push_back(imageNames.size() );
        cout << imageNames.size() << " pictures under this directory" << endl;

        for(int j = 0; j < imageNames.size() ; j++){
            images_train.push_back(imageNames[j]);
         }   
        cout << endl;
    }
    cout<< images_train.size() << " images for trainning"<<endl<<endl;

    int id = 0;
    myImage image[images_train.size()];
    for(int i = 0; i < train_dirNames.size(); i++){
        for(int j = 0; j < topicSize[i] ; j++){
            // cout <<id <<endl;
            image[id].setIndex(id);
            image[id].setLabel( labelNames[i] );
            image[id].setPath( images_train[id] );
            id = id+1;
         }   
        // cout << endl;
    }

    // for(int i = 0; i < images_train.size(); i++){
    //     image[i].getIndex();
    //     image[i].getLabel();
    //     image[i].getPath();
    //     cout << endl;
    // }


    cout << "====================================== "<< endl;
    cout << "Total images "<<images_train.size() << endl;
    // for(int i = 0; i < images_train.size() ; i++)
    //     cout << images_train[i] << endl;
    cout << "====================================== "<< endl;

    vector< cv::Mat > features = loadFeatures(images_train,descriptor);
    vector< cv::Mat > features_query = loadFeatures(images_query,descriptor);
 /////////////////////////////////////////////////////////////////////////////////
    testVocCreation(features);
    creatDatabase(features);
    queryDatabase(features_query,image,images_query);
    cout << "====================================== "<< endl;
    selfEvaluate(features,image);
    cout << "====================================== "<< endl;
///////////////////////////////////////////////////////////////////////////////////////
    return 0;
}