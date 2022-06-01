#ifndef CPP_QT_Q_PAIR_COMPARER_H
#define CPP_QT_Q_PAIR_COMPARER_H

#include <QPair>

// https://stackoverflow.com/a/10189611/12619942
struct QPairFirstComparer
{
	template <typename T1, typename T2>
	bool operator()(const QPair<T1, T2> &a, const QPair<T1, T2> &b) const
	{
		return a.first < b.first;
	}
};

struct QPairSecondComparer
{
	template <typename T1, typename T2>
	bool operator()(const QPair<T1, T2> &a, const QPair<T1, T2> &b) const
	{
		return a.second < b.second;
	}
};

#endif // CPP_QT_Q_PAIR_COMPARER_H