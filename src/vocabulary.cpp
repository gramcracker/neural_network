#include "vocabulary.h"
#include <iostream>
using namespace std;



void vocabulary::parse()
{

    while(cin>>temp)
		{
            read((temp));
			insert((temp));
			if (cin.get()=='\n') break;//terminating character
		}

}
void vocabulary::insert(string word)
{
    if(_Net.layers[0].size()<word_vec.size())
            {
                _Net.AddFullyConnectedNuerons((word_vec.size()-_Net.layers[0].size()),0);
                cout<<endl<<"sentence size "<<word_vec.size();
                cout<<endl<<"layer size "<<_Net.layers[0].size();
            }
    int i=0;

		if (word_vec.size()==0)
		{
			word_vec.push_back (word);


		}else {
			while( i<word_vec.size())
		{

			if(word==word_vec.at(i))
			{
				break;
			}else if(i==word_vec.size()-1)
			{

				word_vec.push_back(word);
				if(_Net.layers[0].size()==word_vec.size())break;
				else{_Net.AddFullyConnectedNuerons(1,0);}
				break;

			}
		i++;
		}
		}
}


void vocabulary::read(string word)
{
    if(window.size()<window_size)
		{
			window.push_back(word);
			cout<<endl<<"vector resized";
		}
		else{

			for(int i=0; i<window.size(); i++)
			{

				if(i==window.size()-1)
				{
					cout<<endl<<"last word";
					window[i]=word;
				}
				else{
					cout<<endl<<"word shifted down";
					window[i]=window[i+1];
				}
			}
		}

}



void vocabulary::display()
	{
		for (vector<std::string>::iterator i = word_vec.begin(); i != word_vec.end(); i++)
		{
			std::cout << *i << std::endl;
		}
	}


