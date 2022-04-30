#include <dirent.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <stdlib.h>

#include <string.h>
#include <stdio.h>

#include "file_browser.h"


int glsl_filename_ending(FILE_T* f) {
	int r = 0;

	if(f && f->length >= 5) {
		r = f->name[f->length-1] == 'l' &&
			f->name[f->length-2] == 's' &&
			f->name[f->length-3] == 'l' &&
			f->name[f->length-4] == 'g' &&
			f->name[f->length-5] == '.';
	}

	return r;
}

void init_files(STATE st) {
	for(u32 i = 0; i < st->max_num_files; i++) {
		st->files[i].length = 0;
		st->files[i].name[0] = '\0';
	}
}

void craft_permission_string(FILE_T* f, mode_t mode) {
	if(f) {

		int p[] = {
			S_IRWXU,
			S_IRUSR,
			S_IWUSR,
			S_IXUSR,

			S_IRWXG,
			S_IRGRP,
			S_IWGRP,
			S_IXGRP,

			S_IRWXO,
			S_IROTH,
			S_IWOTH,
			S_IXOTH
		};

		char symbols[] = {
			'r', 'w', 'x'
		};

		for(int i = 0; i < 12; i++) {

			char c = '.';

			if(mode & p[i]) {
				c = symbols[i%3];
			}

			f->perm_str[i] = c;
		}
	}
}

void scan_directory(STATE st) {

	st->num_files = 0;

	DIR* dir_p = NULL;
	struct dirent* dp = NULL;
	struct stat sb;

	dir_p = opendir(st->current_dir);

	if(!dir_p) {
		return;
	}


	st->total_num_files = 0;

	while((dp = readdir(dir_p)) != NULL) {
		if(dp->d_name[0] == '.') { continue; }
		
		st->total_num_files++;
		


		if(st->total_num_files < st->scroll_y) {
			continue;
		}

		if(st->num_files >= st->max_num_files) {
			continue;
		}

		FILE_T* f = &st->files[st->num_files];
		if(!f) { continue; }

		if(stat(dp->d_name, &sb) < 0) {
			f->length = 0;
			continue;
		}
		
		int b = sb.st_mode & S_IFMT;
		if(b != S_IFREG && b != S_IFDIR) { continue; }
	
		u32 len = strlen(dp->d_name);
		len = (len > FILE_T_NAME_LENGTH) ? FILE_T_NAME_LENGTH : len;
		
		f->length = len;
		f->can_exec = 0;
		memmove(f->name, dp->d_name, len);
		f->name[len] = '\0';

		
		if(b == S_IFDIR) {
			f->is_dir = 1;
			f->r = 0.4;
			f->g = 0.4;
			f->b = 0.8;
		}
		else {
			f->is_dir = 0;
			if(glsl_filename_ending(f)) {
				f->r = 0.2;
				f->g = 0.9;
				f->b = 0.56;
			}
			else {
				if(sb.st_mode & S_IXUSR ||
					   	sb.st_mode & S_IXGRP ||
						sb.st_mode & S_IXOTH) {
					f->r = 0.76;
					f->g = 0.45;
					f->b = 0.52;
					f->can_exec = 1;
				}
				else {
					f->r = 0.55;
					f->g = 0.45;
					f->b = 0.28;
				}
			}
		}

		/*
		if(b == S_IFDIR) {
			f->is_dir = 1;
		}

		f->r = 1.0;
		f->g = 1.0;
		f->b = 1.0;

		st->files_ptr_mask[st->num_files] = f;
		*/

		craft_permission_string(f, sb.st_mode);
		st->num_files++;
	}

	st->max_scroll_y = st->total_num_files;
	closedir(dir_p);
}


