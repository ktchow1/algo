#include<iostream>
#include<idx_seq.h>


// ************ //
// *** Test *** //
// ************ //
void test_template_index_sequence()
{
    static_assert(std::is_same_v<    idx_seq_generator <5>   ::type, idx_seq<0,1,2,3,4>>,        "failed to generate idx_seq");
    static_assert(std::is_same_v<    idx_seq_generator <7>   ::type, idx_seq<0,1,2,3,4,5,6>>,    "failed to generate idx_seq");
    static_assert(std::is_same_v<    idx_seq_generator <8>   ::type, idx_seq<0,1,2,3,4,5,6,7>>,  "failed to generate idx_seq");
    static_assert(std::is_same_v<    idx_seq_generator2<5>   ::type, idx_seq<0,1,2,3,4>>,        "failed to generate idx_seq");
    static_assert(std::is_same_v<    idx_seq_generator2<7>   ::type, idx_seq<0,1,2,3,4,5,6>>,    "failed to generate idx_seq");
    static_assert(std::is_same_v<    idx_seq_generator2<8>   ::type, idx_seq<0,1,2,3,4,5,6,7>>,  "failed to generate idx_seq");
    static_assert(std::is_same_v<inv_idx_seq_generator <5>   ::type, idx_seq<4,3,2,1,0>>,        "failed to generate rev idx_seq");
    static_assert(std::is_same_v<inv_idx_seq_generator <7>   ::type, idx_seq<6,5,4,3,2,1,0>>,    "failed to generate rev idx_seq");
    static_assert(std::is_same_v<inv_idx_seq_generator <8>   ::type, idx_seq<7,6,5,4,3,2,1,0>>,  "failed to generate rev idx_seq");
    static_assert(std::is_same_v<inv_idx_seq_generator2<5>   ::type, idx_seq<4,3,2,1,0>>,        "failed to generate rev idx_seq");
    static_assert(std::is_same_v<inv_idx_seq_generator2<7>   ::type, idx_seq<6,5,4,3,2,1,0>>,    "failed to generate rev idx_seq");
    static_assert(std::is_same_v<inv_idx_seq_generator2<8>   ::type, idx_seq<7,6,5,4,3,2,1,0>>,  "failed to generate rev idx_seq");
    static_assert(std::is_same_v<off_idx_seq_generator <6,7> ::type, idx_seq<6>>,                "failed to generate off idx_seq");
    static_assert(std::is_same_v<off_idx_seq_generator <3,8> ::type, idx_seq<3,4,5,6,7>>,        "failed to generate off idx_seq");
    static_assert(std::is_same_v<off_idx_seq_generator <6,13>::type, idx_seq<6,7,8,9,10,11,12>>, "failed to generate off idx_seq");
    static_assert(std::is_same_v<off_idx_seq_generator2<6,7> ::type, idx_seq<6>>,                "failed to generate off idx_seq");
    static_assert(std::is_same_v<off_idx_seq_generator2<3,8> ::type, idx_seq<3,4,5,6,7>>,        "failed to generate off idx_seq");
    static_assert(std::is_same_v<off_idx_seq_generator2<6,13>::type, idx_seq<6,7,8,9,10,11,12>>, "failed to generate off idx_seq");


    using T0 = push_front_idx_seq<idx_seq<0,1,2,3,5,7,9>,11,12,13>::type;
    static_assert(std::is_same_v<T0, idx_seq<13,12,11,0,1,2,3,5,7,9>>, "failed to push_front_idx_seq");

    using T1 = push_front_idx_seq<idx_seq<>,97,98,99>::type;
    static_assert(std::is_same_v<T1, idx_seq<99,98,97>>, "failed to push_front_idx_seq");

    using T2 = push_back_idx_seq<idx_seq<0,1,2,3,5,7,9>,11,12,13>::type;
    static_assert(std::is_same_v<T2, idx_seq<0,1,2,3,5,7,9,11,12,13>>, "failed to push_back_idx_seq");

    using T3 = push_back_idx_seq<idx_seq<>,97,98,99>::type;
    static_assert(std::is_same_v<T3, idx_seq<97,98,99>>, "failed to push_back_idx_seq");

    using T4 = pop_front_idx_seq<idx_seq<13,0,1,2,3,5,7,9>>::type;
    static_assert(std::is_same_v<T4, idx_seq<0,1,2,3,5,7,9>>, "failed to pop_front_idx_seq");   
  
    using T5 = pop_front_idx_seq<idx_seq<99>>::type;
    static_assert(std::is_same_v<T5, idx_seq<>>, "failed to pop_front_idx_seq");
    
    using T6 = pop_N_front_idx_seq<idx_seq<11,12,13,0,1,2,3,5,7,9>,3>::type;
    static_assert(std::is_same_v<T6, idx_seq<0,1,2,3,5,7,9>>, "failed to pop_N_front_idx_seq");
    
    using T7 = pop_N_front_idx_seq<idx_seq<97,98,99>,3>::type;
    static_assert(std::is_same_v<T7, idx_seq<>>, "failed to pop_N_front_idx_seq");
  
    using T8 = reverse_idx_seq<idx_seq<1,2,3,4,5,6>>::type;
    static_assert(std::is_same_v<T8, idx_seq<6,5,4,3,2,1>>, "failed to reverse_idx_seq");

    using T9 = reverse_idx_seq<idx_seq<80,70,60,50,40,30,20,10>>::type;
    static_assert(std::is_same_v<T9, idx_seq<10,20,30,40,50,60,70,80>>, "failed to reverse_idx_seq");

    using T10 = odd_pick_idx_seq<idx_seq<10,20,30,40,50,60,70,80,90>>::type;
    static_assert(std::is_same_v<T10, idx_seq<10,30,50,70,90>>, "failed to odd_pick_idx_seq");

    using T11 = odd_pick_idx_seq<idx_seq<10>>::type;
    static_assert(std::is_same_v<T11, idx_seq<10>>, "failed to odd_pick_idx_seq");

    using T12 = dedupe_idx_seq<idx_seq<>>::type;
    static_assert(std::is_same_v<T12, idx_seq<>>, "failed to dedupe_idx_seq");

    using T13 = dedupe_idx_seq<idx_seq<1,1,1,1,1>>::type;
    static_assert(std::is_same_v<T13, idx_seq<1>>, "failed to dedupe_idx_seq");

    using T14 = dedupe_idx_seq<idx_seq<1,1,1,1,1,2,3,3,3,4>>::type;
    static_assert(std::is_same_v<T14, idx_seq<1,2,3,4>>, "failed to dedupe_idx_seq");

    using T15 = dedupe_idx_seq<idx_seq<1,2,3,4,5,6,7>>::type;
    static_assert(std::is_same_v<T15, idx_seq<1,2,3,4,5,6,7>>, "failed to dedupe_idx_seq");

    using T16 = dedupe_idx_seq<idx_seq<1,1,1,2,3,3,3,4,4,4,5,6,7,7,7>>::type;
    static_assert(std::is_same_v<T16, idx_seq<1,2,3,4,5,6,7>>, "failed to dedupe_idx_seq");

    using T17 = dedupe_idx_seq<idx_seq<1,1,1,1,1,1,2,3,3,3,3,3,4,4,4,5,6,6,7,7,7,7>>::type;
    static_assert(std::is_same_v<T17, idx_seq<1,2,3,4,5,6,7>>, "failed to dedupe_idx_seq");
}


void test_template_index_sequence_Maven()
{
    using T0 = idx_seq<1,2,3,4,5>;
    using T1 = idx_seq<4,5,6,7,8>;
    using T2 = dot_product<T0,T1>::type;
    using ANS = idx_seq<1*4,2*5,3*6,4*7,5*8>;
    static_assert(std::is_same_v<T2,ANS>, "failed to dot_product");

    using T3 = zip_product<idx_seq<1,2,3,4,5>>::type;
    static_assert(std::is_same_v<T3, idx_seq<1,2,3,4,5>>, "failed to zip product");

    using T4 = zip_product<idx_seq<1,2,3,4,5>, 
                           idx_seq<2,3,4,5,6>>::type;
    static_assert(std::is_same_v<T4, idx_seq<1*2,2*3,3*4,4*5,5*6>>, "failed to zip product");

    using T5 = zip_product<idx_seq<1,2,3,4,5>, 
                           idx_seq<2,3,4,5,6>, 
                           idx_seq<3,4,5,6,7>>::type;
    static_assert(std::is_same_v<T5, idx_seq<1*2*3,2*3*4,3*4*5,4*5*6,5*6*7>>, "failed to zip product");

    using T6 = zip_product<idx_seq<1,2,3,4,5>, 
                           idx_seq<2,3,4,5,6>, 
                           idx_seq<3,4,5,6,7>,
                           idx_seq<4,5,6,7,8>>::type;
    static_assert(std::is_same_v<T6, idx_seq<1*2*3*4,2*3*4*5,3*4*5*6,4*5*6*7,5*6*7*8>>, "failed to zip product");
}
