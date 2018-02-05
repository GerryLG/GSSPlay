#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gssapi/gssapi.h>

int main(int argc, char *argv[argc])
{
  gss_name_t gssname;
  OM_uint32 maj_stat, min_stat;
  gss_buffer_desc namebuf, outnamebuf;
  const char *name = "wibble";

  namebuf.value = (void *)name;
  namebuf.length = strlen(name);

  maj_stat = gss_import_name(&min_stat, &namebuf, GSS_C_NT_HOSTBASED_SERVICE, &gssname);
  if (maj_stat != GSS_S_COMPLETE) {
    fprintf(stderr, "import_name failed with status %d:%d\n", maj_stat, min_stat);
    exit(1);
  }

  maj_stat = gss_display_name(&min_stat, gssname, &outnamebuf, NULL);
  if (maj_stat != GSS_S_COMPLETE) {
    fprintf(stderr, "display_name failed with status %d:%d\n", maj_stat, min_stat);
    exit(1);
  }

  printf("Display name: %s\n", (char *)outnamebuf.value);

  maj_stat = gss_release_buffer(&min_stat, &outnamebuf);

  gss_name_t gssnamedup;
  maj_stat = gss_duplicate_name(&min_stat, gssname, &gssnamedup);

  maj_stat = gss_release_name(&min_stat, &gssname);
  maj_stat = gss_release_name(&min_stat, &gssnamedup);

  return 0;
}
