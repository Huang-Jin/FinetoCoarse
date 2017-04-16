/*
* This class was designed for the ftc algorithm with the operation it needed.
* author: Jin Huang,Julie Delon
* date: 2016/12/17
*/

#ifndef _HIST_LIST_H_
#define _HIST_LIST_H_

#include <iostream>
#include <fstream>
#include <string>

class CHistList
{
public:
	CHistList();
	~CHistList();

	bool Init(int allocsize);											/* Initialize the list with allocsize */
	bool Init(double *data, int size, int allocsize);					/* Initialize the list with data */
	bool Copy(CHistList & hlist);										/* Copy the list */
	void Destroy();														/* Destroy the list */
	bool Pushback(double val);											/* Push value to the end of the list */
	bool Replace(double val, int i);									/* Replace the value in position i with val */

	void Display();														/* Display the list to the screen */
	void Reset();														/* Reset the list with 0 */
	void Delete(int a, int b);											/* Delete some value from the list */
	void Interval(CHistList& out, int a, int b);						/* Get interval [a,b] */

	double & operator[](int i);											/* Rewrite the operator [] as right value*/
	int   GetSize(){ return m_size; }									/* Get the size of the list */
	void  SetSize(int size){ m_size = size; }							/* Get the size of the list */
	void  SetCircle(){ m_bCircle = true; };

	double Sum();														/* Get sum of the list */
	double MaxValue();													/* Get maximum of the list */
	double MeanValue();													/* Get mean value of the list */
	bool IsCircle(){ return m_bCircle; }								/* Whether the list is circle or not */
	void SaveasTxt(std::string path);									/* Save the list into a text file */

	bool m_bBeginfromMin;												/* The list is begin from minimum or not */
private:
	int m_size;															/* Number of samples */
	int m_allocsize;													/* Size allocated (in bytes) */
	double *m_pvalues;													/* The samples */
	bool m_bCircle;														/* The list is circle or not */
};

#endif