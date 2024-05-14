#pragma once

// ***************** //
// *** Algorithm *** //
// ***************** //
// 64644 + 6428 
// ----------------- //
// 0.0 Arithmetic     6 
// 0.1 Bisection      4
// 0.2 Vector         6
// 0.3 Stack          4
// 0.4 List           4
// 0.5 Tree           6
// 0.6 Tree variant   4
// 0.7 Graph          2
// 0.8 Sorting        8
// ----------------- //
// 1. Topological sorting 
// 2. Dijkstra algorithm 


//                  best      aver      worst     inplace  stable  iterator
// --------------------------------------------------------------------------------
//  select          O(N^2)    O(N^2)    O(N^2)    yes      no      forward
//  bubble          O(N)      O(N^2)    O(N^2)    yes      yes     bi-directional
//  insert          O(N)      O(N^2)    O(N^2)    yes      yes     bi-directional
//  quick           O(NlogN)  O(NlogN)  O(N^2)    yes      no      bi-directional
//  merge           O(NlogN)  O(NlogN)  O(NlogN)  no       yes     random access
//  heap            O(NlogN)  O(NlogN)  O(NlogN)  yes      no      random access
//  pigeon          O(N)      O(N)      O(N)      no       yes     forward
//  topological

