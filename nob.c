#define NOB_IMPLEMENTATION
#include "nob.h"

#define BIN_FOLDER "bin/"
#define BUILD_FOLDER "build/"
#define SRC_FOLDER "src/"

int main(int argc, char **argv)
{
	NOB_GO_REBUILD_URSELF(argc, argv);

	if (!nob_mkdir_if_not_exists(BIN_FOLDER))
		return 1;
	if (!nob_mkdir_if_not_exists(BUILD_FOLDER))
		return 1;

	Nob_Cmd cmd = { 0 };

	nob_cc(&cmd);
	nob_cc_flags(&cmd);
	nob_cmd_append(&cmd, "-ggdb");
	nob_cc_inputs(&cmd, SRC_FOLDER "main.c");
	nob_cc_inputs(&cmd, SRC_FOLDER "tga.c");
	nob_cc_inputs(&cmd, SRC_FOLDER "renderer.c");

	nob_cmd_append(&cmd, "-lm");
	nob_cc_output(&cmd, BIN_FOLDER "renderer");

	if (!nob_cmd_run(&cmd))
		return 1;

	return 0;
}

