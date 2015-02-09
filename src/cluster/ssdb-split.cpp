/*
Copyright (c) 2012-2015 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "split.h"

void welcome(){
	printf("ssdb-split - Split ssdb-server\n");
	printf("Copyright (c) 2012-2015 ssdb.io\n");
	printf("\n");
}

void usage(int argc, char **argv){
	printf("Usage:\n");
	printf("    %sr\n", argv[0]);
	printf("\n");
	printf("Options:\n");
	printf("    xx - \n");
}

int main(int argc, char **argv){
	welcome();
	
	Split split;
	split.init("127.0.0.1", 9000, "127.0.0.1", 8888, "127.0.0.1", 8889);
	
	int64_t src_dbsize;
	ssdb::Status s;
	s = split.src_client->dbsize(&src_dbsize);
	if(!s.ok()){
		fprintf(stderr, "error!\n");
		exit(1);
	}
	
	printf("src dbsize: %lld\n", src_dbsize);
	
	int64_t total_moved = 0;
	std::string min_key;
	std::string max_key;
	while(1){
		int64_t size = split.move_some();
		if(size == -1){
			fprintf(stderr, "error!\n");
			exit(1);
		}
		if(size == 0){
			fprintf(stderr, "end.\n");
			split.finish();
			break;
		}
		total_moved += size;
		
		int64_t src_dbsize_new;
		ssdb::Status s;
		s = split.src_client->dbsize(&src_dbsize_new);
		if(!s.ok()){
			fprintf(stderr, "error!\n");
			exit(1);
		}
		
		printf("src_dbsize_old: %lld, src_dbsize_new: %lld\n", src_dbsize, src_dbsize_new);
		getchar();
	}
	
	return 0;
}

