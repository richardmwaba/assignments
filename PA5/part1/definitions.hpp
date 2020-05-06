#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#define PAGE_MASK 0xFF00
#define FRAME_MASK 0xFF00
#define PAGE_NUM_BIT 8
#define FRAME_NUM_BIT 8
#define OFFSET_MASK 0xFF

#define PAGE_TABLE_ENTRIES 256
#define PAGE_TABLE_SIZE 256
#define PHYSICAL_MEMORY_SIZE 256
#define TLB_SIZE 16
#define FRAME_SIZE 256
#define PAGE_SIZE 256
#define NUM_OF_FRAMES 256
#define VALID 1
#define INVALID 0

const std::string OutputFilename = "correct.txt";

struct TranslationLookAsideBuffer {
    int page_number = -1;
    int frame_number = -1;
    int valid = 0;
};

struct PageTable {
    int frame_number = -1;
    int valid = 0;
};

/***********************************************************
 * Function: get_page_number - get the page number from the logical address
 * Parameters: logical_address
 * Return Value: page number
 ***********************************************************/
int get_page_number(int logical_address);
/***********************************************************
 * Function: get_page_offset - get the page offset from the logical address
 * Parameters: logical_address
 * Return Value: offset
 ***********************************************************/
int get_page_offset(int logical_address);
/***********************************************************
 * Function: get_physical_address - get the physical address from the extracted frame number and offset
 * Parameters: frame_num, offset
 * Return Value: physical_address
 ***********************************************************/
int get_physical_address(int frame_num, int offset);
/***********************************************************
 * Function: get_frame_number_from_TLB - query TLB by page number and get frame number 
 * Parameters: page_num
 * Return Value: frame_number_TLB
 ****************************************************/
int get_frame_number_from_TLB(int page_num);
/***********************************************************
 * Function: get_frame_number_from_page_table - query page table by page number and get frame number 
 * Parameters: page_num
 * Return Value: frame_number_page
 ****************************************************/
int get_frame_number_from_page_table(int page_num);
/***********************************************************
 * Function: get_page_from_backing_store - get copy of page from backing store and write page to physical memory
 * Parameters: page_num, position, *backing_store
 * Return Value: new_frame_number
 ****************************************************/
int get_page_from_backing_store_to_memory(int page_num, int position, FILE *backing_store);
/***********************************************************
 * Function: update_page_table - update page table
 * Parameters: page_num, frame_num
 * Return Value: non
 ****************************************************/
void update_page_table(int page_num, int frame_num);
/***********************************************************
 * Function: update_TLB - update TLB
 * Parameters: page_num, frame_num
 * Return Value: non
 ****************************************************/
void update_TLB(int page_num, int frame_num);
/***********************************************************
 * Function: read_byte_from_physical_memory - read content of physical memory location
 * Parameters: frame_num, offset
 * Return Value: value read
 ****************************************************/
int read_byte_from_physical_memory(int frame_num, int offset);
/***********************************************************
 * Function: calculate_and_print_statistics - calculate and print program statics to file
 * Parameters: *output
 * Return Value: non
 ****************************************************/
void calculate_and_print_statistics(FILE *output);

#endif