#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static void fcopy(char *buf, char *mem, int len, char *dst)
{
	if (strncasecmp(buf, mem, len))
		return;

	strcpy(dst, buf + len + 1);
}

static void wget(int low, int high, int end, char *str)
{
	char buf[256];

	while (fgets(buf, 256, stdin) != NULL) {
		printf("P: 99 %d %s\n", end, str ? str : "");
	}
	printf("P: 100 %d %s\n", end, str ? str : "");
}

static void getdeps(int argc, char **argv)
{
	FILE *fp;
	char *pkgsfile = argv[2];
	char buf[1024];
	char **pkgs;
	int i, nr = argc - 3;
	int hit;

	pkgs = malloc(sizeof(char *) * nr);
	for (i = 0; i < nr; i++)
		pkgs[i] = argv[i + 3];

	fp = fopen(pkgsfile, "r");

	hit = 0;
	while (fgets(buf, 1024, fp) != NULL) {
		int len = strlen(buf) - 1;

		if (*buf && buf[len] == '\n')
			buf[len] = '\0';
		if (strncasecmp(buf, "Package:", 8) == 0) {
			int any = 0;

			hit = 0;
			for (i = 0; i < nr; i++) {
				if (!pkgs[i])
					continue;
				any = 1;
				if (strcmp(buf + 9, pkgs[i]) == 0) {
					pkgs[i] = NULL;
					hit = 1;
					break;
				}
			}
			if (!any)
				break;
		} else if (hit) {
			char *p = NULL;

			if (strncasecmp(buf, "Depends:", 8) == 0)
				p = buf + 9;
			else if (strncasecmp(buf, "Pre-Depends:", 12) == 0)
				p = buf + 13;
			if (!p)
				continue;

			while (*p) {
				while (isspace(*p))
					++p;
				while (*p && *p != '(' && *p != '|' &&
						*p != ',' && !(isspace(*p))) {
					putchar(*p++);
				}
				putchar('\n');
				while (*p && *p++ != ',')
					;
			}
		}
	}

	fclose(fp);

	free(pkgs);
}

static void pkgs(int argc, char **argv)
{
	FILE *fp;
	char *field;
	char *mirror;
	char *pkgsfile;
	char buf[1024];
	char ppkg[1024], pver[1024], parc[1024], pfile[1024];
	char pmd5[1024], psize[1024], pval[1024];
	char **pkgs;
	int i, nr = argc - 4;
	int flen, uniq;

	if (strcmp(argv[1], "PKGS") == 0) {
		mirror = argv[2];
		pkgsfile = argv[3];
		field = "Package:";
		flen = 8;
		nr = argc - 4;
		uniq = 1;
	} else {
		mirror = argv[3];
		pkgsfile = argv[4];
		field = argv[2];
		flen = strlen(field);
		nr = argc - 5;
		uniq = 0;
	}

	pkgs = malloc(sizeof(char *) * nr);
	for (i = 0; i < nr; i++)
		pkgs[i] = argv[i + 5 - uniq];

	fp = fopen(pkgsfile, "r");

	while (fgets(buf, 1024, fp) != NULL) {
		int len = strlen(buf) - 1;

		if (*buf && buf[len] == '\n')
			buf[len] = '\0';

		fcopy(buf, "Package:", 8, ppkg);
		fcopy(buf, "Version:", 8, pver);
		fcopy(buf, "Architecture:", 13, parc);
		fcopy(buf, "Size:", 5, psize);
		fcopy(buf, "MD5sum:", 7, pmd5);
		fcopy(buf, "Filename:", 9, pfile);
		fcopy(buf, field, flen, pval);
		if (!*buf) {
			for (i = 0; i < nr; i++) {
				if (!pkgs[i])
					continue;
				if (strcmp(pval, pkgs[i]) == 0) {
					printf("%s %s %s %s %s %s %s\n",
						ppkg, pver, parc, mirror,
						pfile, pmd5, psize);
					if (uniq)
						pkgs[i] = NULL;
					break;
				}
			}
		}
	}
	if (uniq) {
		for (i = 0; i < nr; i++) {
			if (pkgs[i])
				printf("%s -\n", pkgs[i]);
		}
	}

	fclose(fp);

	free(pkgs);
}

static void stanzas(int argc, char **argv)
{
	FILE *fp;
	char *pkgsfile = argv[2];
	char buf[1024];
	char ppkg[1024];
	char *accum;
	char **pkgs;
	int i, nr = argc - 3;
	int asize = 0;
	int memsize = 8192;

	accum = malloc(memsize);

	pkgs = malloc(sizeof(char *) * nr);
	for (i = 0; i < nr; i++)
		pkgs[i] = argv[i + 3];

	fp = fopen(pkgsfile, "r");

	while (fgets(buf, 1024, fp) != NULL) {
		int len = strlen(buf);

		if (len > 0) {
			if (asize + len >= memsize) {
				memsize = asize += 8192;
				accum = realloc(accum, memsize);
			}
			strcpy(accum + asize, buf);
			asize += len;
		}

		--len;
		if (*buf && buf[len] == '\n')
			buf[len] = '\0';

		fcopy(buf, "Package:", 8, ppkg);
		if (!*buf) {
			for (i = 0; i < nr; i++) {
				if (strcmp(ppkg, pkgs[i]) == 0) {
					puts(accum);
					if (accum[asize - 1] != '\n')
						puts("\n");
					else if (accum[asize - 2] != '\n')
						putchar('\n');
					break;
				}
			}
			*accum = '\0';
			asize = 0;
		}
	}

	fclose(fp);

	free(pkgs);
	free(accum);
}

int main(int argc, char **argv)
{
	if (strcmp(argv[1], "WGET%") == 0)
		wget(atoi(argv[2]), atoi(argv[3]),atoi(argv[4]),
			argc == 6 ? argv[5] : NULL);
	else if (strcmp(argv[1], "GETDEPS") == 0)
		getdeps(argc, argv);
	else if (strcmp(argv[1], "PKGS") == 0)
		pkgs(argc, argv);
	else if (strcmp(argv[1], "FIELD") == 0)
		pkgs(argc, argv);
	else if (strcmp(argv[1], "STANZAS") == 0)
		stanzas(argc, argv);

	return 0;
}
