/************************************************************
************************************************************/


#pragma once

/************************************************************
************************************************************/
#define TITLE__FAITHLESS	0
#define TITLE__TIESTO_465	1
#define	TITLE__TIESTO_471	2
#define	TITLE__TIESTO_485	3
	#define TITLE TITLE__TIESTO_485

/************************************************************
************************************************************/

/**************************************************
**************************************************/
struct COLOR_THEME{
	int time_ms;
	int Color_id;
};

/**************************************************
**************************************************/
class COLOR_THEME_TABLE{
	private:
		COLOR_THEME * Table;
		bool b_Last;
		int id;
	
	public:
		COLOR_THEME_TABLE();
		~COLOR_THEME_TABLE();
		
		int IsColorThemeChange(int now_ms);
		void reset();
		bool IsColorThemeChange_soon(int now_ms);
};




