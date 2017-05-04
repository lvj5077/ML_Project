#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <vector>
using namespace std;



vector<string> getFiles(string cate_dir);

int main(){
  string dir;
  // dir=getstring();
  cout<<"The dir is:"<< endl;
  cin>>dir;
  cout << dir <<endl;
  vector<string> vectordirs; 
  vectordirs = getFiles(dir);
 // cout<<vectordirs<<endl;
  return 0;
}

vector<string> getFiles(string cate_dir)
{
    vector<string> files;//存放文件名

        DIR *dir;
	struct dirent *ptr;
//	char base[1000];
 
	if ((dir=opendir(cate_dir.c_str())) == NULL)
        {
		perror("Open dir error...");
                exit(1);
        }
 
	while ((ptr=readdir(dir)) != NULL)
	{

              if(ptr->d_name[0] == '.')
              continue;
          cout<<ptr->d_name<<endl;
          files.push_back(ptr->d_name);

/*		if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir
		        continue;
		else if(ptr->d_type == 8)    ///file
	                cout<<ptr->d_name<<endl;
                        
          //		printf("d_name:%s/%s\n",basePath,ptr->d_name);
			files.push_back(ptr->d_name);
		else if(ptr->d_type == 10)    ///link file
		//	printf("d_name:%s/%s\n",basePath,ptr->d_name);
			continue;
		else if(ptr->d_type == 4)    ///dir
		{
			files.push_back(ptr->d_name);
			memset(base,'\0',sizeof(base));
		        strcpy(base,basePath);
		        strcat(base,"/");
		        strcat(base,ptr->d_nSame);
		        readFileList(base);
			
                }	*/
          
	}
	closedir(dir);

	//排序，按从小到大排序
//	sort(files.begin(), files.end());
	return files;
}
