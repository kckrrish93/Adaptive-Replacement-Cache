/* *************************
ADAPTIVE REPLACEMENT CACHE ALGORITHM
Author: Krishna Prasad K C 
GitHub user name: kckrrish93
Email ID: kckrrish93@gmail.com

ARC Cache consists of 4 lists
TOP1 = Most recently used pages of List 1.
BOTTOM1 = Least recently used pages of List 1.
TOP2 = Most recently used pages of List 2.
BOTTOM2 = Least recently used pages of List 2.
*/
#include<iostream>
#include<list>
using namespace std;

list<int> TOP1,BOTTOM1,TOP2,BOTTOM2;
list<int>::iterator list_iterator;
unsigned int cacheSize = 8;
int p=0;

enum Page_available_location
{
	B1 = 0,
	B2 = 1
};

int calculateP(enum Page_available_location p_loc)
{
	if (p_loc == B1)
	{
		cout<<"\nPage availabe in B1\n";
		p=(float)min((float)cacheSize, (float)(p + max((BOTTOM2.size()*1.0) / BOTTOM1.size(), 1.0)));
	}
	else if (p_loc == B2)
	{
		cout<<"\nPage availabe in B2\n";
		p=(float)max((float)0.0, (float)(p - max((BOTTOM1.size()*1.0) / BOTTOM2.size(), 1.0)));	
	}
	return p;
}

bool page_availability_check(list<int>ARU_list,int requested_page)
{
	if(!ARU_list.empty())
	{
		list_iterator = ARU_list.begin();
		while(list_iterator!= ARU_list.end())
		{
			if(requested_page == *list_iterator)
			{
				return true;
			}
			list_iterator++;
		}
	}
	return false;	
}

void ARC(const int x)
{
	cout<<"Requested page is "<<x<<endl;
	if(TOP1.size() == 0)
	{
		TOP1.push_front(x);	
	}
	else
	{
		bool page_available_in_TOP1,page_available_in_TOP2,page_available_in_BOTTOM1,page_available_in_BOTTOM2 = false;
		
		//Check whether page is available in TOP1.
		page_available_in_TOP1 = page_availability_check(TOP1,x);
		
		//If requested page is not available in TOP1, check the page in TOP2.
		if(!page_available_in_TOP1)
		{
			page_available_in_TOP2 = page_availability_check(TOP2,x);
		}
		//If requested page is not available in TOP1 and TOP2, check the page in BOTTOM1.
		if((!page_available_in_TOP1) && (!page_available_in_TOP2))
		{
			page_available_in_BOTTOM1 = page_availability_check(BOTTOM1,x);
		}
		//If requested page is not available in TOP1,TOP2 and BOTTOM1, check the page in BOTTOM2.
		if((!page_available_in_TOP1) && (!page_available_in_TOP2) && (!page_available_in_BOTTOM1))
		{
			page_available_in_BOTTOM2 = page_availability_check(BOTTOM2,x);
		}				
		
		if(page_available_in_TOP1)	//If page is available in TOP1, move it to MRU of TOP2.
		{
			TOP1.remove(x);
			TOP2.push_front(x);	
		}
		else if(page_available_in_TOP2)	//If page is available in TOP2, move it to MRU of TOP2.
		{
			TOP2.remove(x);
			TOP2.push_front(x);
		}
		//If page is available in BOTTOM1
		else if(page_available_in_BOTTOM1)	
		{
			p = calculateP(B1);	//Calculate P value.
			cout<<"\n value of p is "<<p<<" Size of T1: "<<TOP1.size()<<endl;
			if(TOP1.size() <= p)	//If size of TOP1 <= p, remove the LRU page in TOP2 and move it to BOTTOM2.
			{
				BOTTOM2.push_front(TOP2.back());
				TOP2.pop_back();				
			}
			else					//If size of TOP1 > p, remove the LRU page in TOP1 and move it to BOTTOM1.
			{
				BOTTOM1.push_front(TOP1.back());
				TOP1.pop_back();				
			}
			BOTTOM1.remove(x);		//Remove the page from Bottom1.
			TOP2.push_front(x);		//Move the page to MRU of T2.	
		}
		//If page is available in BOTTOM2
		else if(page_available_in_BOTTOM2)
		{
			p = calculateP(B2);
			cout<<"\n value of p is "<<p<<" Size of T1: "<<TOP1.size()<<endl;
			if(TOP1.size() >= p)	//If size of TOP1 >= p, remove the LRU page in TOP1 and move it to BOTTOM1.
			{
				BOTTOM1.push_front(TOP1.back());
				TOP1.pop_back();
			}
			else					//If size of TOP1 < p, remove the LRU page in TOP2 and move it to BOTTOM2.
			{
				BOTTOM2.push_front(TOP2.back());
				TOP2.pop_back();				
			}
			BOTTOM2.remove(x);	//Remove the page from Bottom2.
			TOP2.push_front(x);	//Move the page to MRU of T2.				
		}		
		else	//If page is not available in any of TOP1, TOP2, BOTTOM1 and BOTTOM2, then add it to MRU of TOP1.
		{
				if((TOP1.size() + TOP2.size()) == cacheSize)	//If TOP1 U TOP2 becomes Cache size
				{
					//If cache directory becomes twice the cache size.
					if((TOP1.size() + TOP2.size() + BOTTOM1.size() + BOTTOM2.size()) == 2*cacheSize)
					{
						BOTTOM1.pop_back();	//Remove LRU Page from Bottom1.
					}
					
					BOTTOM1.push_front(TOP1.back());	//Move the LRU of TOP1 to MRU of Bottom1.
					TOP1.pop_back();
				}			
				TOP1.push_front(x);
		}
	}
	
	cout<<"Pages in TOP1 are: ";
	for(list_iterator = TOP1.begin();list_iterator!=TOP1.end();list_iterator++)
	{
		cout<<"\t"<<*list_iterator;
	}
	cout<<"\nPages in TOP2 are: ";
	for(list_iterator = TOP2.begin();list_iterator!=TOP2.end();list_iterator++)
	{
		cout<<"\t"<<*list_iterator;
	}	
	cout<<"\nPages in BOTTOM1 are: ";
	for(list_iterator = BOTTOM1.begin();list_iterator!=BOTTOM1.end();list_iterator++)
	{
		cout<<"\t"<<*list_iterator;
	}		
	cout<<"\nPages in BOTTOM2 are: ";
	for(list_iterator = BOTTOM2.begin();list_iterator!=BOTTOM2.end();list_iterator++)
	{
		cout<<"\t"<<*list_iterator;
	}
}

int main()
{
	cout<<"Adaptive Replacement Cache"<<endl;
	char request_continue_status;
	int x = 0; //Page to be requested
	while(1)
	{
		cout<<"Enter the page to be requested"<<endl;
		cin>>x;
		ARC(x);
		cout<<"\nDo you want to continue the request (Y/N))?"<<endl;
		cin>>request_continue_status;
		if(request_continue_status == 'n' || request_continue_status == 'N')
		{
			break;
		}				
	}			
	return 0;
}
