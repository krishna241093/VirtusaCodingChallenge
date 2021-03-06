#include <iostream>
#include <map>
#include <string>
#include <cstring>
#include <utility>
#include <list>

using namespace std;

class matchScheduler
{
	private:
		int **mpMatchesArray;
		map<int, string> mTeamsMap;
		map<int, list< pair<string, string> > > mScheduledMatchesMap;
		int iTotalNoOfScheduledDaysForUniqueMatches;
		int iTotalTeamsCount;
		
		void storeTheSchedulesPerDay();

	public:
		matchScheduler()
		{
		}
		~matchScheduler()
		{
			for(unsigned int i = 0; i < iTotalTeamsCount; i++)
				delete [] mpMatchesArray[i];
			delete [] mpMatchesArray;
		}
		void parseTeams(std::string sTeams);
		void doScheduling();
		void displayTheSchedules();
};

void matchScheduler::parseTeams(std::string sTeams)
{
	cout<<"Entering parseTeams\n";
	int iTeamId = 1;
	char *strTeam = strtok(const_cast<char*>(sTeams.c_str()), ",");
	while(strTeam != NULL)
	{
		mTeamsMap.insert(std::make_pair(iTeamId++, std::string(strTeam)));
		strTeam = strtok(NULL, ",");
	}
	iTotalNoOfScheduledDaysForUniqueMatches = mTeamsMap.size() - 1;
	iTotalTeamsCount = mTeamsMap.size(); 
	
	mpMatchesArray = new int*[iTotalTeamsCount];
	for(unsigned int i = 0; i < iTotalTeamsCount; i++)
		mpMatchesArray[i] = new int[iTotalTeamsCount];
	cout<<"Exiting parseTeams\n";
}

void matchScheduler::doScheduling()
{
	cout<<"Entering doScheduling\n";
	int iMatchDay = 0;
	for(unsigned int i = 0; i < iTotalTeamsCount - 1; i++)
	{
		for(unsigned int j = 0; j < iTotalTeamsCount; j++)
		{			
			iMatchDay++;
			if(iMatchDay == iTotalTeamsCount)
				iMatchDay = 1;
			if(i == j)
			{
				mpMatchesArray[iTotalTeamsCount-1][j] = iMatchDay;
			}
			else
			{
				mpMatchesArray[i][j] = iMatchDay;
			}
		}
	}

	storeTheSchedulesPerDay();
	cout<<"Exiting doScheduling\n";
}

void matchScheduler::storeTheSchedulesPerDay()
{	
	cout<<"Entering storeTheSchedulesPerDay\n";
	int iMatchDay = 0;
	//One match with other team (home)
	for(unsigned int i = 0; i < iTotalTeamsCount; i++)
	{
		for(unsigned int j = 0; j < i; j++)
		{
			iMatchDay = mpMatchesArray[i][j];
			map<int, list< pair<string, string> > >::iterator it = mScheduledMatchesMap.find(iMatchDay);
			if(it != mScheduledMatchesMap.end())
			{
				it->second.push_back(make_pair(mTeamsMap[i+1], mTeamsMap[j+1]));
			}
			else
			{
				list< pair<string, string> > tmpList;
				tmpList.push_back(make_pair(mTeamsMap[i+1], mTeamsMap[j+1]));
				mScheduledMatchesMap.insert(make_pair(iMatchDay , tmpList));
			}
		}
	}
	
	//two matches with other team (away)
	for(unsigned int i = 0; i < iTotalTeamsCount; i++)
	{
		for(unsigned int j = i + 1 ; j < iTotalTeamsCount; j++)
		{
			iMatchDay = mpMatchesArray[i][j] + iTotalNoOfScheduledDaysForUniqueMatches;
			map<int, list< pair<string, string> > >::iterator it = mScheduledMatchesMap.find(iMatchDay);
			if(it != mScheduledMatchesMap.end())
			{
				it->second.push_back(make_pair(mTeamsMap[i+1], mTeamsMap[j+1]));
			}
			else
			{
				list< pair<string, string> > tmpList;
				tmpList.push_back(make_pair(mTeamsMap[i+1], mTeamsMap[j+1]));
				mScheduledMatchesMap.insert(make_pair(iMatchDay , tmpList));
			}
		}
	}
	cout<<"Exiting storeTheSchedulesPerDay\n";
}

void matchScheduler::displayTheSchedules()
{
	cout<<"Entering displayTheSchedules\n";
	map<int, list< pair<string, string> > >::iterator it;
	for( it = mScheduledMatchesMap.begin(); it != mScheduledMatchesMap.end(); it++ )
	{
		list< pair<string, string> >::iterator itList;
		for(itList = it->second.begin(); itList != it->second.end(); itList++)
		{
			cout<<"("<<(*itList).first<<","<<(*itList).second<<")";
		}
		cout<<endl;
	}
	cout<<"Exiting displayTheSchedules\n";
}

int main()
{
	cout<<"Enter the teams names with comma separated:";
	std::string sTeams;
	cin >> sTeams;
	
	matchScheduler objMatchScheduler;
	
	objMatchScheduler.parseTeams(sTeams);
	
	objMatchScheduler.doScheduling();
	
	objMatchScheduler.displayTheSchedules();
	
	return 0;
	
}
