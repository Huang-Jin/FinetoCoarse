#include "ftc.h"

double entrop(double x, double y)
{
	if (x == 0.0) return (-(float)log10(1 - y));
	else  if (x == 1.0) return (-(float)log10(y));
	else return (float)(x*log10(x / y) + (1.0 - x)*log10((1.0 - x) / (1.0 - y)));
}

void pooling_adjacent_violators(bool bInc, CHistList& in, CHistList& out)
{
	double som;
	int size, i, j, k;

	size = in.GetSize();
	out.Init(size);
	out.SetSize(size);

	if (!bInc)		// decreasing hypothesis case
	{
		out[0] = in[0];
		for (i = 1; i < size; i++)
		{
			out[i] = in[i];
			som = out[i];
			for (j = i - 1; j >= -1; j--)
			{
				// replace the increasing area with medium when the value of one node is large than medium.
				// it deals with the judging problem occurs when calculating the medium of the increasing region.
				if (j == -1 || (out[j] * (i - j) >= som))
				{
					som /= (float)(i - j);
					for (k = j + 1; k <= i; k++)
						out[k] = som;
					break;
				}
				som += out[j];
			}
		}
	}

	else		// increasing hypothesis case
	{
		out[size - 1] = in[size - 1];
		for (i = size - 2; i >= 0; i--)
		{
			out[i] = in[i];
			som = out[i];
			for (j = i + 1; j <= size; j++)
			{
				if (j == size || (out[j] * (j - i) >= som))
				{
					som /= (float)(j - i);
					for (k = i; k <= j - 1; k++)
						out[k] = som;
					break;
				}
				som += out[j];
			}
		}
	}
}

double max_entropy(bool bInc, CHistList& in, int a, int b, double eps)
{
	CHistList extrait, decrease;
	double seuil, H, r, p, max_entrop, N;
	int i, j, L;

	in.Interval(extrait, a, b);

	// get the decreasing(increasing) law p
	pooling_adjacent_violators(bInc, extrait, decrease);
	L = extrait.GetSize();

	// sum the two list
	for (i = 1; i < L; i++)
	{
		extrait[i] += extrait[i - 1];
		decrease[i] += decrease[i - 1];
	}

	// meaningfulness threshold
	N = extrait[L - 1];
	seuil = (log((float)L*(L + 1) / 2) + eps*log(10)) / (float)N;

	// search the most meaningful segment
	max_entrop = 0.;
	for (i = 0; i < L; i++)
		for (j = i; j < L; j++) {
			if (i == 0) r = (float)extrait[j];
			else r = (float)extrait[j] - extrait[i - 1];
			r = r / (float)N;
			if (i == 0) p = decrease[j];
			else p = decrease[j] - decrease[i - 1];
			p = p / (float)N;
			H = entrop(r, p);
			if (H > max_entrop) {
				max_entrop = H;
			}
		}
	max_entrop = (max_entrop - seuil)*N;

	extrait.Destroy();
	decrease.Destroy();

	return max_entrop;
}

void merge(CHistList& in, float eps, CHistList& list, int jseg)
{
	int i, imin1;
	int m, M;
	int size = in.GetSize();
	int step = 2 * jseg - 1;		// we merge jseg neighboring unimodal intervals at one time.
	double min_max_entrop, H;

	if (list.GetSize() <= step)
		return;

	CHistList max_entrop;
	if (!in.IsCircle())
	{
		max_entrop.Init(list.GetSize() - step);
		max_entrop.SetSize(list.GetSize() - step);
	}
	else
	{
		max_entrop.Init(list.GetSize());
		max_entrop.SetSize(list.GetSize());
		max_entrop.SetCircle();
	}

	// initialzie the entrop list.
	for (i = 0; i < list.GetSize(); i++)
	{
		if (i >= list.GetSize() - step && !in.IsCircle())
			continue;

		// minimum at i
		if ((list.m_bBeginfromMin && i % 2 == 0) || (!list.m_bBeginfromMin && i % 2 == 1))
		{
			m = list[i];
			M = list[i + step];
			max_entrop[i] = max_entropy(true, in, m, M, eps);
		}
		else				// maximum at i -> configuration (min at i+1, max at i+2) in 'list'
		{
			M = list[i];
			m = list[i + step];
			max_entrop[i] = max_entropy(false, in, M, m, eps);
		}
	}

	// Look for the "easiest" merging of two intervals with the related value in the entrop list
	min_max_entrop = max_entrop[0]; imin1 = 0;
	for (i = 0; i < max_entrop.GetSize(); i++)
	{
		H = max_entrop[i];
		if (min_max_entrop > H) { min_max_entrop = H; imin1 = i; }
	}

	//max_entrop.Display();

	// Merge successively pairs of intervals
	while (min_max_entrop < 0)
	{
		max_entrop.Delete(imin1 + 1, step - 1);
		list.Delete(imin1 + 1, step - 1);
		//max_entrop.Display();
		//list.Display();

		if (max_entrop.GetSize() <= 0)
			break;

		// update of max_entrop
		for (i = imin1 - (step - 1); i <= imin1; i++)
		{
			if ((i < 0 || i >= max_entrop.GetSize()) && !in.IsCircle())
				continue;

			if ((list.m_bBeginfromMin && i % 2 == 0) || (!list.m_bBeginfromMin && i % 2 == 1))
			{
				m = list[i];
				M = list[i + step];
				max_entrop[i] = max_entropy(true, in, m, M, eps);
			}
			else
			{
				M = list[i];
				m = list[i + step];
				max_entrop[i] = max_entropy(false, in, M, m, eps);
			}
		}

		min_max_entrop = max_entrop[0]; imin1 = 0;
		for (i = 0; i < max_entrop.GetSize(); i++)
		{
			H = max_entrop[i];
			if (min_max_entrop > H) { min_max_entrop = H; imin1 = i; }
		}
	}

	//std::cout << "\nlist of separators (maxima):\n";
	//for (i = 0; i < list.GetSize(); i ++)
	//	if ((list.m_bBeginfromMin && i % 2 == 1) || (!list.m_bBeginfromMin && i % 2 == 0))
	//		std::cout << list[i] << " ";
	//std::cout << std::endl;
	//list.Display();
	max_entrop.Destroy();
}

void GetLocalMinMax(CHistList& in, CHistList& out)
{
	int i, j;
	int size = in.GetSize();
	out.Init(in.GetSize());
	out.SetCircle();

	i = 0;
	if (!in.IsCircle())
	{
		out.Pushback(0);
		// add the zero into the list
		if (in[0] < in[1]) {
			out.m_bBeginfromMin = true;
		}
		else if (in[1] < in[0])  {
			out.m_bBeginfromMin = false;
		}
		else
		{
			for (j = 1; (j < size) && (in[j] == in[0]); j++) {}
			if (j == size) { return; }						/*case of a constant histogram*/
			else if (in[j] > in[0]) {
				out.m_bBeginfromMin = true;
			}
			else {
				out.m_bBeginfromMin = false;
			}
			i = j;
		}
	}

	for (; i < size; i++)
	{
		if ((i == 0 || i == size - 1) && !in.IsCircle())
		{
			continue;
		}

		// strict minimum
		if ((in[i] < in[i - 1]) && (in[i] < in[i + 1]))
			out.Pushback(i);

		// large minimum
		else if ((in[i] < in[i - 1]) && (in[i] == in[i + 1]))
		{
			if (!in.IsCircle())
			{
				for (j = i + 1; (j < size) && (in[j] == in[i]); j++) {}
				if (j == size)
				{
					out.Pushback(size - 1);
				}
				else if (in[j] > in[i]) {
					out.Pushback(0.5*(i + j - 1));
				}
			}
			else
			{
				for (j = i + 1; (j < i + size) && (in[j] == in[i]); j++) {}
				if (in[j] > in[i]) {
					out.Pushback(0.5*(i + j - 1));
				}
			}
			i = j - 1;
		}

		// strict maximum
		else if ((in[i] > in[i - 1]) && (in[i] > in[i + 1]))
			out.Pushback(i);

		// large maximum
		else if ((in[i] > in[i - 1]) && (in[i] == in[i + 1]))
		{
			if (!in.IsCircle())
			{
				for (j = i + 1; (j < size) && (in[j] == in[i]); j++) {}
				if (j == size)
				{
					out.Pushback(size - 1);
				}
				else if (in[j] < in[i]) {
					out.Pushback(0.5*(i + j - 1));
				}
			}
			else
			{
				for (j = i + 1; (j < size) && (in[j] == in[i]); j++) {}
				if (in[j] < in[i]) {
					out.Pushback(0.5*(i + j - 1));
				}
			}
			i = j - 1;
		}
	}

	if (!in.IsCircle() && out[out.GetSize() - 1] != size - 1)
		out.Pushback(size - 1);

	if (in.IsCircle())
	{
		if (in[out[0]] < in[out[1]])
		{
			out.m_bBeginfromMin = true;
		}
	}
}

void FTCSegment(float eps, CHistList& in, CHistList& out)
{
	CHistList list;
	list.Init(in.GetSize());

	// step 1: get the original list.
	// get the local minimums and maximums to generate the original list.
	GetLocalMinMax(in, list);
	printf("List of all maximas-minima (with the bounds 0 and size-1):\n");
	list.Display();

	// when the size of the list is less than 4 , it means there is only an unimodal area. 
	if (list.GetSize() < 4)
	{
		printf("unimodal histogram !\n");
		return;
	}

	// step 2: merge process.
	// when 2 neighbor intervals could be considered as unimodal, they will be merged.
	merge(in, eps, list, 2);

	// step 3: confirm there is no more than j segments that could be merged.
	//int len = list.GetSize() / 2;
	//for (int j = 3; j <= len; j++)
	//{
	//	merge(in, eps, list, j);
	//}

	//list.Display();
	// optimization --> find the new center between two maxima.
	for (int i = 0; i < list.GetSize(); i++)
	{
		if (!in.IsCircle() && i >= list.GetSize() - 2)
		{
			continue;
		}
		if ((list.m_bBeginfromMin && i % 2 == 1) || (!list.m_bBeginfromMin && i % 2 == 0))
		{
			double min_sum = 0;
			int min_j = -1;
			for (int j = list[i] + 1; j < list[i + 2]; j++)
			{
				double dec = max_entropy(false, in, list[i], j, eps);
				double inc = max_entropy(true, in, j, list[i + 2], eps);
				if (dec < 0 && inc < 0)
				{
					if (dec + inc < min_sum)
					{
						min_sum = dec + inc;
						min_j = j;
					}
				}
			}
			if (min_j != -1)
			{
				list[i + 1] = min_j;
			}
		}
	}

	list.Display();
	// fill the output list.
	out.Init(list.GetSize());
	if (in.IsCircle())
		out.SetCircle();
	for (int i = 0; i < list.GetSize(); i++)
		if ((list.m_bBeginfromMin && i % 2 == 0) || (!list.m_bBeginfromMin && i % 2 == 1))
			out.Pushback(list[i]);
	out.Display();
}
