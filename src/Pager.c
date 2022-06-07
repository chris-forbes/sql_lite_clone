#include "Pager.h"
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

void pager_flush(Pager* pager, u32 page_num, u32 size) {
  if(pager->pages[page_num] == NULL) {
    printf("Tried to flush null page\n");
    exit(EXIT_FAILURE);
  }
  off_t offset = lseek(pager->file_descriptor, page_num * PAGE_SIZE, SEEK_SET);

  if(offset == -1) {
    printf("Error seeking: %d\n", errno);
    exit(EXIT_FAILURE);
  }
  ssize_t bytes_written =
    write(pager->file_descriptor, pager->pages[page_num], size);
  if(bytes_written == -1) {
    printf("Error during page flush to disk\n") ;
    exit(EXIT_FAILURE);
  }
}

Pager* pager_open(const char* filename) {
  int file_descriptor = open(filename, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
  if(file_descriptor == -1) {
    printf("Unable to open database file\n");
    exit(EXIT_FAILURE);
  }

  off_t file_length = lseek(file_descriptor, 0, SEEK_END);
  Pager* pager = (Pager*)malloc(sizeof(Pager));
  pager->file_descriptor = file_descriptor;
  pager->file_length = file_length;
  for(u32 i = 0; i < TABLE_MAX_PAGES; i++) {
    pager->pages[i] = NULL;
  }
  return pager;
}

void* get_page(Pager* pager, const u32 page_num){
  if(page_num > TABLE_MAX_PAGES) {
    printf("Attempt to fetch page number out of bounds: pageno %d of %d \n", page_num, TABLE_MAX_PAGES);
    exit(EXIT_FAILURE);
  }
  // All pages in the pager are initialised as null, this is our read cache,
  // If its null this is a cache miss so we're going to have to goto disk to get
  // the page from our DB file
  if(pager->pages[page_num] == NULL) {
    void* page = malloc(PAGE_SIZE);
    u32 num_pages = pager -> file_length / PAGE_SIZE;

    //funny edge case, we might need to save partial pages at the end of the file
    //so make sure we count that as a full page
    if(pager -> file_length & PAGE_SIZE) {
      num_pages += 1;
    }
    if(page_num <= num_pages) {
      lseek(pager->file_descriptor, page_num * PAGE_SIZE, SEEK_SET);
      ssize_t bytes_read = read(pager->file_descriptor, page, PAGE_SIZE);
      if(bytes_read == -1) {
	printf("Error reading file: %d\n", errno);
	exit(EXIT_FAILURE);
      }
    }
    pager -> pages[page_num] = page;
  }
  return pager->pages[page_num];
}
