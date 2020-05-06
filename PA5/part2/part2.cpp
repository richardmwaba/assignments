/*********************************************************************************
 * Author: Richard Mwaba
 * Title: Designing a Virtual Memory Manager
 * Date: Monday, April 28, 2020
 * Description: This programmes illustrates how a virtual memory manager works by
 * implementing one using arrays.
 * This part includes FIFO and LRU page replacement algorithms Page table. TLB
 * always uses FIFO in this program
 *********************************************************************************/

#include <iostream>
// #include <cctype>
#include<stack>

#include "definitions.hpp"

using namespace std;
/***********************************************************
 * Global variables: variables globally accessible in the program
 ***********************************************************/
TranslationLookAsideBuffer TLB[TLB_SIZE];
PageTable Page_Table[PAGE_TABLE_SIZE];
int Physical_Memory[PHYSICAL_MEMORY_SIZE][FRAME_SIZE];

FILE *backing_store_fp;
stack<int>free_frames;
int tlb_index = 0, fifo_index = 0, global_clock = 0;
int TLB_hits = 0, TLB_access = 0, page_table_access = 0; 
int page_table_hits = 0, page_faults = 0, translated_addresses = 0;
bool not_found_in_TLB = true, not_found_in_page_table = true;

/***********************************************************
 * Function: get_page_number - get the page number from the logical address
 * Parameters: logical_address
 * Return Value: page number
 ***********************************************************/
int get_page_number(int logical_address){
    return (logical_address & PAGE_MASK) >> PAGE_NUM_BIT;
}
/***********************************************************
 * Function: get_page_offset - get the page offset from the logical address
 * Parameters: logical_address
 * Return Value: offset
 ***********************************************************/
int get_page_offset(int logical_address){
    return (logical_address & OFFSET_MASK);
}
/***********************************************************
 * Function: get_physical_address - get the physical address from the extracted frame number and offset
 * Parameters: frame_num, offset
 * Return Value: physical_address
 ***********************************************************/
int get_physical_address(int frame_num, int offset){
    return (frame_num << FRAME_NUM_BIT) + offset;
}
/***********************************************************
 * Function: initialise_free_frames - initialise available frames on stack
 * Parameters: none
 * Return Value: none
 ***********************************************************/
void initialise_free_frames(){
    for(int i = NUM_OF_FRAMES-1;i >= 0; i--){
        free_frames.push(i);
    }
}
/***********************************************************
 * Function: free_frame_available - check whether stack has available frame
 * Parameters: none
 * Return Value: true/false
 ***********************************************************/
bool free_frame_available(){
    return !free_frames.empty();
}
/***********************************************************
 * Function: get_free_frame - get a free frame from stack
 * Parameters: non
 * Return Value: free_frame
 ***********************************************************/
int get_free_frame(){
    int free_frame = -1;
    if(free_frames.size() > 0){
        free_frame = free_frames.top();
        free_frames.pop();
    }
    return free_frame;
}
/***********************************************************
 * Function: get_frame_number_from_TLB - query TLB by page number and get frame number 
 * Parameters: page_num
 * Return Value: frame_number_TLB
 ****************************************************/
int get_frame_number_from_TLB(int page_num){
    TLB_access++;
    not_found_in_TLB = true;

    int frame_number_TLB = -1;
    for(int i = 0;i < TLB_SIZE;i++){
        if(page_num == TLB[i].page_number && TLB[i].valid == VALID){
            frame_number_TLB = TLB[i].frame_number;
            not_found_in_TLB = false;
            TLB_hits++;
            Page_Table[page_num].access_time = global_clock;
            global_clock++;
            break;
        }
    }

    return frame_number_TLB;
}
/***********************************************************
 * Function: get_frame_number_from_page_table - query page table by page number and get frame number 
 * Parameters: page_num
 * Return Value: frame_number_page
 ****************************************************/
int get_frame_number_from_page_table(int page_num){
    page_table_access++;

    int frame_number_page = -1;
    if(page_num < PAGE_TABLE_SIZE){
        if( Page_Table[page_num].valid == VALID){
            frame_number_page = Page_Table[page_num].frame_number;
            not_found_in_page_table = false;
            page_table_hits++;
        }else{
            not_found_in_page_table = true;
            page_faults++;
        }
    }else{
        fprintf(stderr, "Virtual memory address out of range");
        exit(EXIT_FAILURE);
    }
    Page_Table[page_num].access_time = global_clock;
    global_clock++;

    return frame_number_page;
}
/***********************************************************
 * Function: get_page_from_backing_store - get copy of page from backing store and write page to physical memory
 * Parameters: position, frame_num, *backing_store
 * Return Value: non
 ****************************************************/
void get_page_from_backing_store_to_memory(int position, int frame_num, FILE *backing_store){
    signed char page_buffer[PAGE_SIZE];
    if(fseek(backing_store, position, SEEK_SET) == -1){
        fprintf(stderr, "Unable to seek to position %d of backing store file", position);
        exit(EXIT_FAILURE);
    }
    if(fread(page_buffer, sizeof(signed char), PAGE_SIZE, backing_store) == -1){
        fprintf(stderr, "Unable to read from backing store file");
        exit(EXIT_FAILURE);
    }

    if(frame_num < NUM_OF_FRAMES){
        for(int i = 0;i < PAGE_SIZE;i++){
            Physical_Memory[frame_num][i] = page_buffer[i];
        }
    }else{
        fprintf(stderr, "Frame number %d is invalid. Total number of frames is %d", frame_num, NUM_OF_FRAMES);
        exit(EXIT_FAILURE);
    }

}
/***********************************************************
 * Function: update_page_table - update page table
 * Parameters: page_num, frame_num, status
 * Return Value: non
 ****************************************************/
void update_page_table(int page_num, int frame_num, int status){
    Page_Table[page_num].frame_number = frame_num;
    Page_Table[page_num].valid = status;
    Page_Table[page_num].access_time = global_clock;
    Page_Table[page_num].queue_number = fifo_index;
    global_clock++;
    fifo_index++;
}
/***********************************************************
 * Function: update_TLB - update TLB
 * Parameters: page_num, frame_num, status
 * Return Value: non
 ****************************************************/
void update_TLB(int page_num, int frame_num, int status){
    TLB[tlb_index].frame_number = frame_num;
    TLB[tlb_index].page_number = page_num;
    TLB[tlb_index].valid = status;

    tlb_index = (tlb_index + 1) % TLB_SIZE;
}
 /* Function: invalidate_TLB_entry - make a TLB entry invalid
 * Parameters: frame_num
 * Return Value: non
 ****************************************************/
void invalidate_TLB_entry(int frame_num){
    for(int i = 0;i < TLB_SIZE;i++){
        if(TLB[i].frame_number == frame_num && TLB[i].valid == VALID){
            TLB[i].valid = INVALID;
            break;
        }
    }
}
/***********************************************************
 * Function: replace_frame_in_page_table_and_TLB - replace page in page table by fifo or lru strategy
 * Parameters: *strategy
 * Return Value: frame_num
 ****************************************************/
int replace_frame_in_page_table_and_TLB(char *strategy){
    int minimum = 0;
    int frame_num = 0;
    int page_num = -1;
    if(fifo.compare(strategy) == 0){
        minimum = fifo_index+1;
        for(int i = 0;i < PAGE_TABLE_SIZE;i++){
            if(Page_Table[i].valid == VALID && Page_Table[i].queue_number < minimum){
                minimum = Page_Table[i].queue_number;
                frame_num = Page_Table[i].frame_number;
                page_num = i;
            }
        }
    } 
    if(lru.compare(strategy) == 0){
        minimum = global_clock+1;
        for(int i = 0;i < PAGE_TABLE_SIZE;i++){
            if(Page_Table[i].valid == VALID && Page_Table[i].access_time < minimum){
                minimum = Page_Table[i].access_time;
                frame_num = Page_Table[i].frame_number;
                page_num = i;
            }
        }
    }
    if(Page_Table[page_num].valid == VALID){
        update_page_table(page_num, frame_num, INVALID);
        invalidate_TLB_entry(frame_num);
    }
    return frame_num;
}
/***********************************************************
 * Function: read_byte_from_physical_memory - read content of physical memory location
 * Parameters: frame_num, offset
 * Return Value: value read
 ****************************************************/
int read_byte_from_physical_memory(int frame_num, int offset){
    return Physical_Memory[frame_num][offset];
}
/***********************************************************
 * Function: calculate_and_print_statistics - calculate and print program statics to file
 * Parameters: *output
 * Return Value: non
 ****************************************************/
void calculate_and_print_statistics(FILE *output){
    fprintf(output, "Number of Translated Addresses = %d\n", translated_addresses);
    double page_fault_rate = page_faults / (double)translated_addresses;
    double TLB_hit_rate = TLB_hits / (double)translated_addresses;

    fprintf(output, "Page Faults = %d\n", page_faults);
    fprintf(output, "Page Fault Rate = %.3f\n", page_fault_rate);
    fprintf(output, "TLB Hits = %d\n", TLB_hits);
    fprintf(output, "TLB Hit Rate = %.3f\n", TLB_hit_rate);
}

int main(int argc, char *argv[]) {
    int logical_address, physical_address;
    int page_number, offset, frame_number;
    int position = 0, value;
    char *strategy;

    if(argc > 3){
        strategy = argv[3];
        if(fifo.compare(strategy) == 0 || lru.compare(strategy) == 0){
            FILE * logical_addresses_file;
            FILE * output_file;
            logical_addresses_file = fopen(argv[2], "r");
            backing_store_fp = fopen(argv[1], "rb");
            output_file = fopen(OutputFilename.c_str(), "w+");

            initialise_free_frames();

            if (logical_addresses_file != NULL) {
                while (fscanf(logical_addresses_file, "%d", &logical_address) != EOF) {
                    page_number = get_page_number(logical_address);
                    offset = get_page_offset(logical_address);
                   
                    frame_number = get_frame_number_from_TLB(page_number);
                   
                    if(not_found_in_TLB){
                        frame_number = get_frame_number_from_page_table(page_number);
                        
                        if(not_found_in_page_table){
                            position = page_number * PAGE_SIZE;
                            if(free_frame_available()){
                                frame_number = get_free_frame();
                            }else{
                                frame_number = replace_frame_in_page_table_and_TLB(strategy);
                            }
                            get_page_from_backing_store_to_memory(position, frame_number, backing_store_fp);
                            update_page_table(page_number, frame_number, VALID);
                        }
                        update_TLB(page_number, frame_number, VALID);
                    }

                    value = read_byte_from_physical_memory(frame_number, offset);
                    physical_address = get_physical_address(frame_number, offset);

                    translated_addresses++;

                    fprintf(output_file, "Virtual address: %d Physical address: %d Value: %d\n", logical_address, physical_address, value);
                }
                calculate_and_print_statistics(output_file);
            }else {
                cout << "Could not open file. Ensure file exists or is readable" << endl;
                exit(EXIT_FAILURE);
            }

            fclose(logical_addresses_file);
            fclose(backing_store_fp);
        }else{
            cout << "Error: Invalid parameter strategy \""<< strategy << "\", fifo or lru should be used" << endl;
            exit(EXIT_FAILURE);
        }
        
    }else{
        cout << "Error: Not enough parameters" << endl;
        cout << "Syntax  - ./part1 <Backing Store> <input file name> <strategy>" << endl;
        exit(EXIT_FAILURE);
    }
}