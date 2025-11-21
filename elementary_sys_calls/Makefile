.PHONY: clean

all: write_to_file append_to_file read_file flush_file

write_to_file: write_to_file.c
	gcc -o write_to_file write_to_file.c

flush_file: flush_file.c
	gcc -o flush_file flush_file.c

append_to_file: append_to_file.c
	gcc -o append_to_file append_to_file.c

read_file: read_file.c
	gcc -o read_file read_file.c

clean:
	rm write_to_file append_to_file flush_file read_file
