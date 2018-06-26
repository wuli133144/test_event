#include <string>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <ctype.h>


bool isvalid_str(string &str){
   std::size_t sz=str.size();
   if(sz<1)return false;
   std::for_each(str.begin(),str.end(),[&](char c){
	   if(!isdigit(c)){
		   return false;
	   }
   });
  return true;
}

