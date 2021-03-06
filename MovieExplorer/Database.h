#pragma once

// NOTE: The ID string of the movie contains the status
// ""            : movie is being updated
// "<id>"        : movie is being updated OR is already updated, just check bUpdated
// "unknown"     : the movie could not be identified
// "connError"   : there was a connection error while updating
// "scrapeError" : an error occurred while scraping movie information

#define DBI_STATUS_NONE			0	// typically an update is in progress
#define DBI_STATUS_UPDATED		1	// movie information updated successfully
#define DBI_STATUS_UNKNOWN		2	// could not find movie in service
#define DBI_STATUS_CONNERROR	3	// there was a connection error whilst retrieving information
#define DBI_STATUS_SCRAPEERROR	4

#define DBI_STAR_NUMBER			3	//number of movie star names 

#define DB_TYPE_UNKNOWN			0
#define DB_TYPE_MOVIE			1
#define DB_TYPE_TV				2
/*
#define DB_SORTBY_TITLEASC		0
#define DB_SORTBY_TITLEDES		1
#define DB_SORTBY_YEARASC		2
#define DB_SORTBY_YEARDES		3
#define DB_SORTBY_RATINGASC		4
#define DB_SORTBY_RATINGDES		5
#define DB_SORTBY_FILEDATEASC	6
#define DB_SORTBY_FILEDATEDES	7
*/

////////////////////////////////////////////////////////////////////////////////////////////////////
// Data structs

struct DBCATEGORY;
struct DBDIRECTORY;
struct DBMOVIE;
struct DBINFO;

struct DBCATEGORY
{
	RString strName;
	RObArray<DBDIRECTORY> directories;
};

struct DBDIRECTORY
{
	RString strPath, strComputerName;
	RObArray<DBMOVIE> movies;
	DBCATEGORY *pCategory;
};

struct DBMOVIE
{
	RString strFileName, strIMDbID, strMovieMeterID, strTitle, strYear, strCountries, strGenres, strContentRating,
			strStoryline, strDirectors, strWriters, strStars, strEpisodeName, strAirDate;
	float fRating, fRatingMax, fIMDbRating, fIMDbRatingMax;
	INT_PTR nVotes, nIMDbVotes, nYear, nMetascore, nSeason, nEpisode, nRuntime;
	BYTE bType;
	RArray<BYTE> posterData;
	RArray<BYTE>* actorImageData[DBI_STAR_NUMBER];
	RString strActorId[DBI_STAR_NUMBER];
	UINT64 fileSize, fileTime, resumeTime;
	bool bSeen, bHide, bUpdated; // would be great to eliminate bUpdated...
	DBDIRECTORY *pDirectory;
};
 
struct DBINFO
{
	RString strServiceName, strID, strIMDbID, strTitle, strYear, strGenres, strContentRating, strCountries,  
			strStoryline, strDirectors, strWriters, strStars, strSearchTitle, strSearchYear, 
			strFileName, strEpisodeName, strAirDate;
	float fRating, fRatingMax, fIMDbRating, fIMDbRatingMax;
	INT_PTR nVotes, nIMDbVotes, nMetascore, nSeason, nEpisode, nRuntime;
	BYTE bType;
	RArray<BYTE> posterData;
	RArray<BYTE>* actorImageData[DBI_STAR_NUMBER];
	RString strActorId[DBI_STAR_NUMBER];
	DWORD status;
	UINT64 timestamp;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// General functions

void ClearInfo(DBINFO *pInfo);
void ClearMovie(DBMOVIE *pMovie);
void TagToInfo(RXMLTag *pTag, DBINFO *pInfo);
void InfoToTag(DBINFO *pInfo, RXMLTag *pTag);
bool GetFirstMatch(RString_ strTarget, RString_ strPattern, RString *pStr1, ...);
bool IsTVEpisode(DBINFO *pInfo);
RString GetStar(RString strStars, int nStar);


////////////////////////////////////////////////////////////////////////////////////////////////////
// CDatabase class declaration

class CDatabase : public RWindow
{
	friend class RWindow;

public:
	CDatabase();
	~CDatabase();
	bool Load(RString_ strFilePath);
	void Unload();
	bool Save();
	void SyncAndUpdate();
	void UpdateResumeTime(RString strFilePath, UINT64 resumeTime);
	void Update();
	void Update(DBMOVIE *pMov);
	void CancelUpdate();
	void FilterByKeywords(RObArray<RString> keywords);
	void FilterByGenres(RObArray<RString> genres);
	void FilterByCategories(RArray<INT_PTR> categories);
	void Filter();
	void SetOnlyTV(bool bFlag);
	void SetOnlyMovies(bool bFlag);

	RArray<DBMOVIE*> m_movies;
	RObArray<DBCATEGORY> m_categories;

protected:
	LRESULT WndProc(UINT Msg, WPARAM wParam, LPARAM lParam);
	void OnPrefChanged();

	RString m_strFilePath;
	RArray<DBMOVIE*> m_updateMovies;
	RArray<HANDLE> m_updateThreads;
	RArray<UINT> m_updateThreadIDs;
	INT_PTR m_nTotalUpdates, m_nUpdatesFromWeb;
	RObArray<RString> m_filterKeywords, m_filterGenres;
	RArray<INT_PTR> m_filterCategories;
	bool m_bShowSeenMovies, m_bShowHiddenMovies, m_bSearchStoryline, m_bShowOnlyTV, m_bShowOnlyMovies;
	UINT_PTR m_sortBy;
};
