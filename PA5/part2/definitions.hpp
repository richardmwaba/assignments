#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#define ADDRESS_SIZE 12
#define PAGE_MASK 0xFF00
#define FRAME_MASK 0xFF00
#define PAGE_NUM_BIT 8
#define FRAME_NUM_BIT 8
#define OFFSET_MASK 0xFF

#define PAGE_TABLE_ENTRIES 256
#define PAGE_TABLE_SIZE 256
#define PHYSICAL_MEMORY_SIZE 128
#define TLB_SIZE 16
#define FRAME_SIZE 256
#define PAGE_SIZE 256
#define NUM_OF_FRAMES 128
#define VALID 1
#define INVALID 0

/***********************************************************
 * Global variables: 
 ***********************************************************/

const std::string OutputFilename = "correct.txt";
const std::string fifo = "fifo";
const std::string lru = "lru";
/***********************************************************
 * Data structure: TranslationLookAsideBuffer - stores TLB attributes
 ***********************************************************/
struct TranslationLookAsideBuffer {
    int page_number = -1;
    int frame_number = -1;
    int valid = 0;
};
/***********************************************************
 * Data structure: PageTable - stores page table attributes
 ***********************************************************/
struct PageTable {
    int frame_number = -1;
    int access_time = 0;
    int queue_number = 0;
    int valid = INVALID;

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
 * Function: initialise_free_frames - initialise available frames on stack
 * Parameters: none
 * Return Value: none
 ***********************************************************/
void initialise_free_frames();
/***********************************************************
 * Function: free_frame_available - check whether stack has available frame
 * Parameters: none
 * Return Value: true/false
 ***********************************************************/
bool free_frame_available();
/***********************************************************
 * Function: get_free_frame - get a free frame from stack
 * Parameters: non
 * Return Value: free_frame
 ***********************************************************/
int get_free_frame();
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
 * Parameters: position, frame_num, *backing_store
 * Return Value: non
 ****************************************************/
void get_page_from_backing_store_to_memory(int position, int frame_num, FILE *backing_store);
/***********************************************************
 * Function: invalidate_TLB_entry - make a TLB entry invalid
 * Parameters: frame_num
 * Return Value: non
 ****************************************************/
void invalidate_TLB_entry(int frame_num);
/***********************************************************
 * Function: replace_frame_in_page_table_and_TLB - replace page in page table by fifo or lru strategy
 * Parameters: *strategy
 * Return Value: frame_num
 ****************************************************/
int replace_frame_in_page_table_and_TLB(char *strategy);
/***********************************************************
 * Function: update_page_table - update page table
 * Parameters: page_num, frame_num, status
 * Return Value: non
 ****************************************************/
void update_page_table(int page_num, int frame_num, int status);
/***********************************************************
 * Function: update_TLB - update TLB
 * Parameters: page_num, frame_num, status
 * Return Value: non
 ****************************************************/
void update_TLB(int page_num, int frame_num, int status);
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