#pragma once
/*
 *  PhraseNode.h
 *  CreateOnDisk
 *
 *  Created by Hieu Hoang on 01/01/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#include <fstream>
#include <vector>
#include <map>
#include "Word.h"
#include "TargetPhraseCollection.h"

namespace OnDiskPt
{

class OnDiskWrapper;
class SourcePhrase;
class PhraseNode;

class Cache
{
#define MAX_PRUNE  500000
	typedef std::pair<Moses::UINT64, Word> CacheKey;
	typedef std::map<CacheKey, Moses::UINT64> CacheNode;
	
	CacheNode m_coll;

public:
	void Add(Moses::UINT64 filePosParent, const Word &wordSought, Moses::UINT64 filePosChild);
	bool Get(Moses::UINT64 filePosParent, const Word &wordSought, Moses::UINT64 &filePosChild);
	void Prune();
	
};
	
	
class PhraseNode
{
	static Cache s_cache;
	
	friend std::ostream& operator<<(std::ostream&, const PhraseNode&);
protected:
	Moses::UINT64 m_filePos, m_value;

	typedef std::map<Word, PhraseNode> ChildColl;
	ChildColl m_children;
	PhraseNode *m_currChild;
	bool m_saved;
	size_t m_pos;
	std::vector<float> m_counts;
	
	TargetPhraseCollection m_targetPhraseColl;
	
	char *m_memLoad, *m_memLoadLast;
	Moses::UINT64 m_numChildrenLoad;

	void AddTargetPhrase(size_t pos, const SourcePhrase &sourcePhrase
											 , TargetPhrase *targetPhrase, OnDiskWrapper &onDiskWrapper
											 , size_t tableLimit, const std::vector<float> &counts);
	size_t ReadChild(Word &wordFound, Moses::UINT64 &childFilePos, const char *mem, size_t numFactors) const;
	void GetChild(Word &wordFound, Moses::UINT64 &childFilePos, size_t ind, OnDiskWrapper &onDiskWrapper) const;

public:
	static size_t GetNodeSize(size_t numChildren, size_t wordSize, size_t countSize);

	PhraseNode(); // unsaved node
	PhraseNode(Moses::UINT64 filePos, OnDiskWrapper &onDiskWrapper); // load saved node
	~PhraseNode();
	
	void Add(const Word &word, Moses::UINT64 nextFilePos, size_t wordSize);
	void Save(OnDiskWrapper &onDiskWrapper, size_t pos, size_t tableLimit);

	void AddTargetPhrase(const SourcePhrase &sourcePhrase, TargetPhrase *targetPhrase
											 , OnDiskWrapper &onDiskWrapper, size_t tableLimit
											 , const std::vector<float> &counts);

	Moses::UINT64 GetFilePos() const
	{ return m_filePos; }
	Moses::UINT64 GetValue() const
	{ return m_value; }
	void SetValue(Moses::UINT64 value)
	{ m_value = value; }
	size_t GetSize() const
	{ return m_children.size(); }

	bool Saved() const
	{ return m_saved; }

	void SetPos(size_t pos)
	{ m_pos = pos; }

	const PhraseNode *GetChild(const Word &wordSought, OnDiskWrapper &onDiskWrapper) const;
	const TargetPhraseCollection *GetTargetPhraseCollection(size_t tableLimit, OnDiskWrapper &onDiskWrapper) const;
	
	void AddCounts(const std::vector<float> &counts)
	{ m_counts = counts; }
	float GetCount(size_t ind) const;
	
};

}

