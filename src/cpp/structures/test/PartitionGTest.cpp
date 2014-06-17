/*
 * PartitionGTest.cpp
 *
 *  Created on: 04.12.2013
 *      Author: Maximilian Vogel (uocvf@student.kit.edu)
 */

#include <iostream>

#include "PartitionGTest.h"

#include "../Partition.h"

#ifndef NOGTEST

namespace NetworKit {

/*TEST_F(PartitionGTest, test*) {
}*/

TEST_F(PartitionGTest, testConstructor) {
	Partition p(10);
	EXPECT_EQ(0,p.lowerBound());
	EXPECT_EQ(1,p.upperBound());
}


TEST_F(PartitionGTest, testAllToSingletons) {
	Partition p(10);
	p.allToSingletons();
	for (int i = 0; i < 10; i++) {
		EXPECT_TRUE(p[i] != p[(i+1)%10]);
	}
}

TEST_F(PartitionGTest, testAddToSubsetSuccess) {
	// Add to Subset really necessary?
	Partition p(10);
	p.addToSubset(0,8);
	p.addToSubset(0,3);
	EXPECT_EQ(p[8],p[3]);	
}

TEST_F(PartitionGTest, testMoveToSubsetSuccess) {
	Partition p(10);
	p.allToSingletons();
	p.moveToSubset(p[3],8);
	EXPECT_EQ(p[8],p[3]);
}

TEST_F(PartitionGTest, testMergeSubsets) {
	Partition p(10);
	p.allToSingletons();
	p.mergeSubsets(p[0],p[9]);
	p.mergeSubsets(p[1],p[8]);
	p.mergeSubsets(p[2],p[7]);
	p.mergeSubsets(p[0],p[1]);
	p.mergeSubsets(p[1],p[2]);
	EXPECT_EQ(p[9],p[7]);
}

/*TEST_F(PartitionGTest, testIsOnePartitionFalse) {
	Partition p(10);
	p.allToSingletons();
	EXPECT_FALSE(p.isOnePartition({}));
}

TEST_F(PartitionGTest, testIsOnePartitionFalse2) {
	Partition p(10);
	p.allToSingletons();
	p.forEntries([&](index e,index s){
		p.moveToSubset(p[0],e);
	});
	p.toSingleton(9);
	EXPECT_FALSE(p.isOnePartition({}));
}

TEST_F(PartitionGTest, testIsOnePartitionTrue) {
	Partition p(10);
	p.allToSingletons();
	p.forEntries([&](index e,index s){
		p.moveToSubset(p[9],e);
	});
	EXPECT_TRUE(p.isOnePartition({}));
}

TEST_F(PartitionGTest, testIsSingletonPartitionTrue) {
	Partition p(10);
	p.allToSingletons();
	EXPECT_TRUE(p.isSingletonPartition({}));
}

TEST_F(PartitionGTest, testIsSingletonPartitionFalse) {
	Partition p(10);
	p.allToSingletons();
	p.mergeSubsets(p[0],p[9]);
	EXPECT_FALSE(p.isSingletonPartition({}));
}*/


TEST_F(PartitionGTest, testNumberOfSubsets) {
	count n = 6542;
	Partition p(n);
	p.allToSingletons();
	EXPECT_EQ(p.numberOfSubsets(),n);
}

TEST_F(PartitionGTest, testNumberOfSubsets2) {
	count n = 6542;
	Partition p(n);
	p.allToSingletons();
	p.parallelForEntries([&](index e, index s) {
		p.moveToSubset(p[0],e);
	});
	EXPECT_EQ(p.numberOfSubsets(),1);
}

TEST_F(PartitionGTest, testNumberOfSubsets3) {
	count n = 6542;
	count subsets = n/2;
	Partition p(n);
	p.allToSingletons();
	for (uint64_t i = 0; i < n; i+=2) {
		p.mergeSubsets(p[i],p[i+1]);
	}
	EXPECT_EQ(p.numberOfSubsets(),subsets);
}
TEST_F(PartitionGTest, testLowerBound) {
	count n = 6542;
	Partition p(n);
	p.allToSingletons();
	EXPECT_EQ(p.lowerBound(),0);
}

TEST_F(PartitionGTest, testUpperBound) {
	count n = 6542;
	Partition p(n);
	p.allToSingletons();
	// ID start with 1, plus 1 for hard upper bound
	count ub = n+1;
	EXPECT_EQ(p.upperBound(),ub);
}

TEST_F(PartitionGTest, testUpperBound2) {
	count n = 6542;
	Partition p(n);
	p.allToSingletons();
	for (int i = 0; i < 6542; i+=2) {
		p.mergeSubsets(p[i],p[i+1]);
	}
	// 6542 because of singletons
	// +(6542/2) = 3271 merge operations
	// +1 for "hard" upper bound = 9814
	EXPECT_EQ(p.upperBound(),9814);
}

TEST_F(PartitionGTest, testContainsSuccessSingletons) {
	Partition p(10);
	p.allToSingletons();
	EXPECT_TRUE(p.contains(8));
}

TEST_F(PartitionGTest, testContainsSuccessAfterMerges) {
	Partition p(10);
	p.allToSingletons();
	p.forEntries([&](index e,index s){
		index n = (e==9)?0:e+1;
		p.mergeSubsets(p[e],p[n]);
	});
	p.toSingleton(9);
	EXPECT_TRUE(p.contains(8));
}

TEST_F(PartitionGTest, testContainsNodeOutOfRange) {
	Partition p(10);
	p.allToSingletons();
	EXPECT_FALSE(p.contains(15));
}

TEST_F(PartitionGTest, testContainsNodeWithoutSubset) {
	Partition p(10);
	for (int i = 0; i < 10; i++) {
		EXPECT_FALSE(p.contains(i));
	}
}

TEST_F(PartitionGTest, testInSameSubsetTrue) {
	Partition p(10);
	p.allToSingletons();
	p.mergeSubsets(p[0],p[9]);
	p.mergeSubsets(p[1],p[8]);
	p.mergeSubsets(p[2],p[7]);
	p.mergeSubsets(p[0],p[1]);
	p.mergeSubsets(p[1],p[2]);
	EXPECT_TRUE(p.inSameSubset(9,7));
}

TEST_F(PartitionGTest, testInSameSubsetFalse) {
	Partition p(10);
	p.allToSingletons();
	p.mergeSubsets(p[0],p[9]);
	p.mergeSubsets(p[1],p[8]);
	p.mergeSubsets(p[2],p[7]);
	p.mergeSubsets(p[0],p[1]);
	p.mergeSubsets(p[1],p[2]);
	EXPECT_FALSE(p.inSameSubset(9,4));
}

TEST_F(PartitionGTest, testCompact) {
	Partition p(10);
	p.allToSingletons();
	p.mergeSubsets(p[0],p[9]);
	p.mergeSubsets(p[1],p[8]);
	p.mergeSubsets(p[2],p[7]);
	p.mergeSubsets(p[0],p[1]);
	p.mergeSubsets(p[1],p[2]);
	p.compact();
	EXPECT_EQ(p.upperBound(),5); // This is only a weak test

	// the following is a deeper test that checks if partition ids and structures match
	std::vector<index> controlSet = {0,0,0,1,2,3,4,0,0,0};
	p.forEntries([&](index e,index s){
		EXPECT_EQ(controlSet[e],s);
	});
} 


TEST_F(PartitionGTest, testSubsetSizes) {
	Partition p(10);
	p.allToSingletons();
	p.mergeSubsets(p[0],p[9]);
	p.mergeSubsets(p[1],p[8]);
	p.mergeSubsets(p[2],p[7]);
	p.mergeSubsets(p[0],p[1]);
	p.mergeSubsets(p[1],p[2]);
	std::vector<count> sizesFunction = p.subsetSizes();
	std::vector<count> sizesControl = {1,1,1,1,6};
	for (index i = 0; i < sizesFunction.size(); i ++) {
		EXPECT_EQ(sizesFunction[i],sizesControl[i]);
	}
}

TEST_F(PartitionGTest, testSubsetSizeMap) {
	Partition p(10);
	p.allToSingletons();
	p.mergeSubsets(p[0],p[9]);
	p.mergeSubsets(p[1],p[8]);
	p.mergeSubsets(p[2],p[7]);
	p.mergeSubsets(p[0],p[1]);
	p.mergeSubsets(p[1],p[2]);
	auto subsizemap = p.subsetSizeMap();
	EXPECT_EQ(subsizemap[p[0]],6);
	EXPECT_EQ(subsizemap[p[3]],1);
	EXPECT_EQ(subsizemap[p[4]],1);
	EXPECT_EQ(subsizemap[p[5]],1);
	EXPECT_EQ(subsizemap[p[6]],1);
}

TEST_F(PartitionGTest, testGetMembers) {
	Partition p(10);
	p.allToSingletons();
	p.mergeSubsets(p[0],p[9]);
	p.mergeSubsets(p[1],p[8]);
	p.mergeSubsets(p[2],p[7]);
	p.mergeSubsets(p[0],p[1]);
	p.mergeSubsets(p[1],p[2]);
	auto members = p.getMembers(p[0]);
	std::vector<index> membersControl = {0,1,2,7,8,9};
	index i = 0;
	for (auto it = members.begin(); it != members.end(); it++) {
		EXPECT_EQ(*it,membersControl[i]);
		i++;
	}
}

TEST_F(PartitionGTest, testNumberOfElements) {
	index n = 10;
	Partition p(n);
	p.allToSingletons();
	p.mergeSubsets(p[0],p[9]);
	p.mergeSubsets(p[1],p[8]);
	p.mergeSubsets(p[2],p[7]);
	p.mergeSubsets(p[0],p[1]);
	p.mergeSubsets(p[1],p[2]);
	EXPECT_EQ(p.numberOfElements(),n);
}




} /* namespace NetworKit */

#endif /*NOGTEST */
