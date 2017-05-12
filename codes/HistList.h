/*
* This class was designed for the ftc algorithm 
* with the operation it needed.
* Author: Jin Huang
* Date: 2017/05/11
*/

#ifndef _HIST_LIST_H_
#define _HIST_LIST_H_

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

class CHistList
{
public:
	CHistList();
	~CHistList();

	/* Initialize the list with allocsize */
	bool Init(int allocsize);

	/* Initialize the list with external data */
	bool Init(double *data, int size, int allocsize);

	/* Copy the list to "hlist"*/
	bool Copy(CHistList & hlist);

	/* Destroy the list */
	void Destroy();

	/* Push value to the end of the list */
	bool Pushback(double val);

	/* Display the list to the screen */
	void Display();	

	/* Reset the content of the list with 0 */
	void Reset();

	/* Delete some value from the list with the interval [a,b] */
	void Delete(int a, int b);

	/* Get interval [a,b] */
	void Interval(CHistList& out, int a, int b);

	/* Override the operator [] as right value */
	double & operator[](int i);

	/* Get the size of the list */
	int   GetSize(){ return m_size; }
	
	/* Set the real size of the list */
	void  SetSize(int size){ m_size = size; }

	/* Set the list to a circle */
	void  SetCircle(){ m_bCircle = true; };

	/* Sum the list */
	double Sum();

	/* Get maximum of the list */
	double MaxValue();

	/* Get mean value of the list */
	double MeanValue();

	/* Whether the list is circle or not */
	bool IsCircle(){ return m_bCircle; }

	/* Save the list into a text file */
	void SaveasTxt(std::string path);

	/* The list is begin from minimum or not */
	bool m_bBeginfromMin;
private:
	/* Number of samples */
	int m_size;
	/* Size allocated */
	int m_allocsize;
	/* The samples */
	double *m_pvalues;
	/* The list is circle or not */
	bool m_bCircle;
};

#endif
