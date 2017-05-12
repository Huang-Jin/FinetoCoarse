/*
 * Copyright (c) 2017-2018, Jin Huang <jin_huang@whu.edu.cn>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under, at your option, the terms of the GNU General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version, or
 * the terms of the simplified BSD license.
 *
 * You should have received a copy of these licenses along this
 * program. If not, see <http://www.gnu.org/licenses/> and
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

#include "HistList.h"

CHistList::CHistList()
{
	m_size = 0;
	m_allocsize = 0;
	m_pvalues = NULL;
	m_bCircle = false;
	m_bBeginfromMin = false;
}

CHistList::~CHistList()
{
	Destroy();
}

bool CHistList::Init(int allocsize)
{
	if (allocsize <= 0)
	{
		return false;
	}

	if (m_pvalues)
	{
		delete[] m_pvalues;
	}

	m_allocsize = allocsize;
	m_size = 0;
	m_pvalues = new double[allocsize];
	memset(m_pvalues, 0, sizeof(double)*allocsize);

	return true;
}

bool CHistList::Init(double *data, int size,int allocsize)
{
	if (data == NULL ||
		size <= 0 ||
		allocsize <= 0 ||
		size > allocsize)
	{
		return false;
	}

	if (m_pvalues)
	{
		delete[] m_pvalues;
	}

	m_allocsize = allocsize;
	m_size = size;
	m_pvalues = new double[allocsize];

	for (int i = 0; i < size;i++)
	{
		m_pvalues[i] = data[i];
	}

	return true;
}

void CHistList::Destroy()
{
	if (m_pvalues != NULL)
	{
		delete[] m_pvalues;
		m_pvalues = NULL;
	}

	m_size = 0;
	m_allocsize = 0;
}

void CHistList::Display()
{
	for (unsigned int i = 0; i < m_size; i++)
	{
		std::cout << m_pvalues[i] << " ";
	}

	std::cout << "\n";
}

bool CHistList::Copy(CHistList & hlist)
{
	if (hlist.m_pvalues)
	{
		hlist.Destroy();
	}

	return hlist.Init(m_pvalues, m_size, m_allocsize);
}

void CHistList::Reset()
{
	if (m_pvalues && m_size > 0)
	{
		memset(m_pvalues, 0, sizeof(float)*m_size);
	}
}

bool CHistList::Pushback(double val)
{
	if (m_allocsize == 0)
		return false;

	m_pvalues[m_size] = val;
	m_size++;

	return true;
}

void CHistList::Interval(CHistList& out, int a, int b)
{
	// deal with the circular list
	if (m_bCircle)
	{
		// when a > b, it means we want to 
		// get the interval [a,size-1] and [0,b]
		if (a > b)
		{
			out.Init(m_size - a + b + 1);
			for (unsigned int i = a; i < m_size; i++)
				out.Pushback(m_pvalues[i]);
			for (unsigned int i = 0; i <= b; i++)
				out.Pushback(m_pvalues[i]);
		}
		else
		{
			out.Init(b - a + 1);
			for (unsigned int i = a; i <= b; i++)
				out.Pushback(m_pvalues[i]);
		}
	}
	// deal with the regular list
	else
	{
		out.Init(b - a + 1);
		for (unsigned int i = a; i <= b; i++)
			out.Pushback(m_pvalues[i]);
	}
}

double& CHistList::operator[](int i)
{
	if (i >= 0 && i < m_size)
		return m_pvalues[i];
	else if (m_bCircle)
	{
		if (i < 0)
		{
			return m_pvalues[i + m_size];
		}
		else
		{
			return m_pvalues[i - m_size];
		}
	}
	else{
		// when get a wrong i, we will kill the process.
		std::cout << "array out of bouds";
		system("pause");
		exit(1);
	}
}

void CHistList::Delete(int start, int num)
{
	if (m_bCircle)
	{
		// if we didn't cross the bound,
		// we only need to delete like regular one.
		if (start <= m_size - num)
		{
			for (int i = start; i < m_size - num; i++)
			{
				m_pvalues[i] = m_pvalues[i + num];
			}
		}
		else
		{
			// while crossed the bound,we only need to delete
			// the beginning of the list because the end part of the
			// list will be deleted by the last sentence "m_size -= num";
			int new_num = start + num - m_size;
			for (int i = 0; i < m_size - new_num; i++)
			{
				m_pvalues[i] = m_pvalues[i + new_num];
			}
		}
	}
	else
	{
		if (start <= m_size - num)
		{
			for (int i = start; i < m_size - num; i++)
			{
				m_pvalues[i] = m_pvalues[i + num];
			}
		}
	}

	m_size-=num;
}

double CHistList::Sum()
{
	if (m_size < 0)
		return -1;

	float sum = 0;
	for (unsigned i = 0; i < m_size;i++)
	{
		sum += m_pvalues[i];
	}

	return sum;
}

double CHistList::MaxValue()
{
	if (m_size < 0)
		return -1;

	float sum = m_pvalues[0];
	for (unsigned i = 1; i < m_size; i++)
	{
		if (sum < m_pvalues[i])
		{
			sum = m_pvalues[i];
		}
	}

	return sum;
}

double CHistList::MeanValue()
{
	return Sum() / GetSize();
}

void CHistList::SaveasTxt(std::string path)
{
	std::ofstream of(path.c_str());

	for (int i = 0; i < m_size; i++)
	{
		of << m_pvalues[i] << " ";
	}
	of.close();
}

