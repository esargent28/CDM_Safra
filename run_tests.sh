#!/bin/bash

# Set up environment, make sure code is compiled
make
mkdir test_results

# Run on basic Buechi test cases
./safra test/buechi1.aut test_results/rabin1.txt
./safra test/buechi2.aut test_results/rabin2.txt
./safra test/buechi3.aut test_results/rabin3.txt
./safra test/buechi4.aut test_results/rabin4.txt

# Run on little monsters
./safra test/littlemonster3.aut test_results/littlemonsterrabin3.txt
./safra test/littlemonster4.aut test_results/littlemonsterrabin4.txt
./safra test/littlemonster5.aut test_results/littlemonsterrabin5.txt

# Run on actual monsters
./safra test/monster3.aut test_results/monsterrabin3.txt
./safra test/monster4.aut test_results/monsterrabin4.txt
./safra test/monster5.aut test_results/monsterrabin5.txt


