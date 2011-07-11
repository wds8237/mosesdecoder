
#pragma once

#include <vector>
#include <set>
#include "../../moses/src/WordsRange.h"
#include "ChartTranslationOption.h"

namespace MosesChart
{

class ChartTranslationOptionOrderer
{
public:
	bool operator()(const TranslationOption* transOptA, const TranslationOption* transOptB) const
	{
		assert(transOptA->GetSourceWordsRange() == transOptB->GetSourceWordsRange());
		return transOptA->GetTotalScore() > transOptB->GetTotalScore();
	}
};

class ChartTranslationOptionUnique
{
public:
	bool operator()(const TranslationOption* transOptA, const TranslationOption* transOptB) const
	{
		assert(transOptA->GetSourceWordsRange() == transOptB->GetSourceWordsRange());
		
		return (*transOptA) < (*transOptB);
	}
};
	
class TranslationOptionList
{
	friend std::ostream& operator<<(std::ostream&, const TranslationOptionList&);

protected:
	
	typedef std::vector<TranslationOption*> CollType;
	CollType m_coll;
	Moses::WordsRange m_range;

public:
	typedef CollType::iterator iterator;
	typedef CollType::const_iterator const_iterator;
	const_iterator begin() const { return m_coll.begin(); }
	const_iterator end() const { return m_coll.end(); }
	iterator begin() { return m_coll.begin(); }
	iterator end() { return m_coll.end(); }

	TranslationOptionList(const Moses::WordsRange &range)
	: m_range(range)
	{}

	~TranslationOptionList();

	size_t GetSize() const
	{ return m_coll.size();	}
	const Moses::WordsRange &GetSourceRange() const
	{ return m_range;	}
	void Add(TranslationOption *transOpt);

	void Sort();

	void DiscardDuplicates();
};

}
