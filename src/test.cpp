#include<iostream>
#include<gtest/gtest.h>

void test_algbase();
void test_allocator();
void test_backtrace();
void test_casting();
void test_casting_shared_ptr();
void test_concepts();
void test_concepts_invocable();
void test_cstr(); 
void test_ctad(); 
void test_decltype();
void test_declval0();
void test_declval1();
void test_deleter();
void test_deleter_for_pool();
void test_enum();
void test_fileio();
void test_filesystem();
void test_function_array_arg();
void test_function_macro();
void test_global_and_static0();
void test_global_and_static1();
void test_gtest(int argc, char* argv[]);
void test_hashfct0();
void test_hashfct1();
void test_hashmap0();
void test_hashmap1();
void test_hashmap2_reference_wrapper();
void test_hashmap3_custom_key();
void test_index_seq0();
void test_index_seq1();
void test_index_seq2();
void test_is_vector();
void test_literal_operator();
void test_macro0();
void test_macro1();
void test_move_semantics();
void test_new_operator();
void test_optional();
void test_parallel_algo();
void test_perfect_forwarding();
void test_placement_new0();
void test_placement_new1();
void test_placement_new2();
void test_pod();
void test_pod_string();
void test_pod_T();
void test_randgen();
void test_range_lib();
void test_reference();
void test_sfinae0();
void test_sfinae1();
void test_sfinae2();
void test_sfinae_class();
void test_sfinae_is_base_implement();
void test_sfinae_is_base_usage();
void test_signal();
void test_signal_and_timer();
void test_simd0();
void test_simd1();
void test_simd_matrix0();
void test_simd_matrix1();
void test_singleton();
void test_STL_constraint0();
void test_STL_constraint1();
void test_STL_constraint2(); /*
void test_STL_constraint3();  */
void test_structured_binding();
void test_template_member();   
void test_template_member_ptr();   
void test_template_template(); 
void test_template_common_type();
void test_template_zip_with();
void test_specialization_for_char_array();
void test_template_traits();
void test_template_AFT();
void test_template_AFT_invocable();
void test_time();
void test_timer_resolution();
void test_tree_traverse();
void test_tuple();
void test_unique_ptr();
void test_unique_ptr_to_noncopyable();
void test_unique_ptr_to_array();
void test_valgrind();
void test_variadic();
void test_variant_and_any();
void test_variant_overloading_lambda();
void test_variant_aggregate_init();
void test_variant_CTAD();
void test_variant_with_decltype();

// hackersrank
void test_anagram();
void test_int2str();        // citadel
void test_matrix_spiral();  // citadel
void test_K_merge();        // citadel UK
void test_tree_checker();   // optiver

// DAL : xml
void test_xml0();
void test_xml1();

#include <template_specialization_in_header.h> // include this header to create<F28> multi-definition error
void test_template_specialization_in_header0();
void test_template_specialization_in_header1();

// YLib : C-poly / event-handler / message-handler / PSQL / reckless / yaml
void test_cat_cut();
void test_cpoly(); 
void test_compose_and_parse();   
void test_event_handler(); 
void test_message_handler(); 
void test_psql0();
void test_psql1();
void test_psql_API();
void test_psql_tuple();
void test_reckless_custom_argument();
void test_reckless_custom_header_and_argument();
void test_yaml0();
void test_yaml1();
void test_yaml2();

int main(int argc, char* argv[])
{
    bool run_all = false; // It is ok after relocation to the UK.
    if (run_all)
    {
        test_algbase();
        test_allocator();
    //  test_backtrace();
        test_casting();
        test_casting_shared_ptr();
        test_concepts();
        test_concepts_invocable();
        test_cstr(); 
        test_ctad();
        test_decltype();
        test_declval0(); 
        test_declval1(); 
        test_deleter();
        test_deleter_for_pool();
        test_enum(); 
        test_fileio();
        test_filesystem(); 
        test_function_array_arg();
        test_function_macro();
        test_global_and_static0();
        test_global_and_static1();
        test_gtest(argc, argv);
        test_hashfct0();
        test_hashfct1(); 
        test_hashmap0(); 
        test_hashmap1(); 
        test_hashmap2_reference_wrapper();
        test_hashmap3_custom_key();
        test_index_seq0();
        test_index_seq1();
        test_index_seq2();
        test_is_vector();
        test_literal_operator(); 
        test_macro0();
    //  test_macro1();
        test_move_semantics(); 
    //  test_new_operator(); 
        test_optional();  
        test_parallel_algo();
        test_perfect_forwarding(); 
        test_placement_new0();
        test_placement_new1();
        test_placement_new2(); 
        test_pod();
        test_pod_string();
        test_pod_T();
        test_randgen();    
        test_range_lib();    
        test_reference();
        test_sfinae0();
        test_sfinae1();
        test_sfinae2(); 
        test_sfinae_class(); 
        test_sfinae_is_base_implement();
        test_sfinae_is_base_usage();
        test_signal();
        test_signal_and_timer();
        test_simd0();
        test_simd1();
        test_simd_matrix0();
        test_simd_matrix1();
        test_singleton();
        test_STL_constraint0();
        test_STL_constraint1();
        test_STL_constraint2();
    //  test_STL_constraint3();
        test_structured_binding(); 
        test_template_member();
        test_template_member_ptr();
        test_template_template(); 
        test_template_common_type();
        test_template_zip_with(); 
        test_specialization_for_char_array();
        test_template_traits();
        test_template_AFT();
        test_template_AFT_invocable();
        test_time();
    //  test_timer_resolution();
        test_tree_traverse(); 
        test_tuple();     
        test_unique_ptr(); 
        test_unique_ptr_to_noncopyable();
        test_unique_ptr_to_array();
        test_valgrind();  
        test_variadic();  
        test_variant_and_any();  
        test_variant_overloading_lambda();
        test_variant_aggregate_init();
        test_variant_CTAD(); 
        test_variant_with_decltype();
    }
    test_fileio();

    // *********** //
    // *** DAL *** //
    // *********** //
//  test_xml0();
//  test_xml1();
//  test_template_specialization_in_header0();
//  test_template_specialization_in_header1();
    
    // ************ //
    // *** YLib *** //
    // ************ //
//  test_cat_cut();
//  test_cpoly();                // POD for C-style polymorphism
//  test_compose_and_parse();    // POD for protocol composing and parsing
//  test_event_handler(); 
//  test_message_handler(); 
//  test_psql0();
//  test_psql1();
//  test_psql_API();
//  test_psql_tuple();
//  test_reckless_custom_argument();
//  test_reckless_custom_header_and_argument();
//  test_yaml0(); 
//  test_yaml1(); 
//  test_yaml2();

    std::cout << "\n\n\n";
    return 0;
}
