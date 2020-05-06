/*********************************************************************************
 * Author: Richard Mwaba
 * Title: Designing a Virtual Memory Manager
 * Date: Monday, April 28, 2020
 * Description: This programmes illustrates how a virtual memory manager works by
 * implementing one using arrays.
 * This part only includes FIFO page replacement in both TLB and Page table
 *********************************************************************************/

#include <iostream>

#include "definitions.hpp"

using namespace std;
/***********************************************************
 * Global variables: variables globally accessible in the program
 ***********************************************************/
TranslationLookAsideBuffer TLB[TLB_SIZE];
PageTable Page_Table[PAGE_TABLE_SIZE];
int Physical_Memory[PHYSICAL_MEMORY_SIZE][FRAME_SIZE];

FILE *backing_store_fp;
int tlb_index = 0, frame_number_index = 0;
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

    return frame_number_page;
}
/***********************************************************
 * Function: get_page_from_backing_store - get copy of page from backing store and write page to physical memory
 * Parameters: page_num, position, *backing_store
 * Return Value: new_frame_number
 ****************************************************/
int get_page_from_backing_store_to_memory(int page_num, int position, FILE *backing_store){
    signed char page_buffer[PAGE_SIZE];
    int new_frame_number = 0;
    if(fseek(backing_store, position, SEEK_SET) == -1){
        fprintf(stderr, "Unable to seek to position %d of backing store file", position);
        exit(EXIT_FAILURE);
    }
    if(fread(page_buffer, sizeof(signed char), PAGE_SIZE, backing_store) == -1){
        fprintf(stderr, "Unable to read from backing store file");
        exit(EXIT_FAILURE);
    }

    new_frame_number = frame_number_index;

    if(new_frame_number < NUM_OF_FRAMES){
        for(int i = 0;i < PAGE_SIZE;i++){
            Physical_Memory[new_frame_number][i] = page_buffer[i];
        }
    }else{
        fprintf(stderr, "Frame number %d is invalid. Total number of frames is %d", new_frame_number, NUM_OF_FRAMES);
        exit(EXIT_FAILURE);
    }

    frame_number_index = (frame_number_index + 1) % NUM_OF_FRAMES;

    return new_frame_number;
}
/***********************************************************
 * Function: update_page_table - update page table
 * Parameters: page_num, frame_num
 * Return Value: non
 ****************************************************/
void update_page_table(int page_num, int frame_num){
    Page_Table[page_num].frame_number = frame_num;
    Page_Table[page_num].valid = VALID;
}
/***********************************************************
 * Function: update_TLB - update TLB
 * Parameters: page_num, frame_num
 * Return Value: non
 ****************************************************/
void update_TLB(int page_num, int frame_num){
    TLB[tlb_index].frame_number = frame_num;
    TLB[tlb_index].page_number = page_num;
    TLB[tlb_index].valid = VALID;

    tlb_index = (tlb_index + 1) % TLB_SIZE;
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
    int page_num, offset, frame_number;
    int position = 0, value;

    if(argc > 2){
        FILE * logical_addresses_fd;
        FILE * output_file;
        logical_addresses_fd = fopen(argv[2], "r");
        backing_store_fp = fopen(argv[1], "rb");
        output_file = fopen(OutputFilename.c_str(), "w+");

        if (logical_addresses_fd != NULL) {
            while (fscanf(logical_addresses_fd, "%d", &logical_address) != EOF) {
                translated_addresses++;

                page_num = get_page_number(logical_address);
                offset = get_page_offset(logical_address);
                frame_number = get_frame_number_from_TLB(page_num);

                if(not_found_in_TLB){
                    frame_number = get_frame_number_from_page_table(page_num);

                    if(not_found_in_page_table){
                        position = page_num * PAGE_SIZE;
                        frame_number = get_page_from_backing_store_to_memory(page_num, position, backing_store_fp);
                        update_page_table(page_num, frame_number);
                    }
                    update_TLB(page_num, frame_number);
                }

                value = read_byte_from_physical_memory(frame_number, offset);
                physical_address = get_physical_address(frame_number, offset);

                fprintf(output_file, "Virtual address: %d Physical address: %d Value: %d\n", logical_address, physical_address, value);
            }
            calculate_and_print_statistics(output_file);
        }else {
            cout << "Could not open file. Ensure file exists or is readable" << endl;
            exit(EXIT_FAILURE);
        }

        fclose(logical_addresses_fd);
        fclose(backing_store_fp);
    }else{
        cout << "Error: Not enough parameters" << endl;
        cout << "Syntax  - ./part1 <Backing Store> <input file name>" << endl;
    }
}