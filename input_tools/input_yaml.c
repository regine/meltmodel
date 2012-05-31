/****************************************************************************/
/* FILE   input_yaml.c
/*        Part of the Hock Melt Model
/*  Author: Lyman Gillispie
/*  Created on: May 29, 2012
/****************************************************************************/
#include <stdio.h>
#include <yaml.h>

int main(void)
{
    FILE *fh = fopen("input.yaml","r");
    yaml_parser_t parser;
    yaml_token_t  token;   /* new variable */

    /* initialize parser */
    if(!yaml_parser_initialize(&parser))
        fputs("Failed to initialize parser~\n", stderr);
    if(fh == NULL)
        fputs("Failed to open file!\n", stderr);

    /* Set input file */
    yaml_parser_set_input_file(&parser, fh);

    /* BEGIN new code */
      do {
        yaml_parser_scan(&parser, &token);
        switch(token.type)
        {
        /* Stream start/end */
        case YAML_STREAM_START_TOKEN: puts("STREAM START"); break;
        case YAML_STREAM_END_TOKEN:   puts("STREAM END");   break;
        /* Token types (read before actual token) */
        case YAML_KEY_TOKEN:   printf("(Key token)   "); break;
        case YAML_VALUE_TOKEN: printf("(Value token) "); break;
        /* Block delimeters */
        case YAML_BLOCK_SEQUENCE_START_TOKEN: puts("<b>Start Block (Sequence)</b>"); break;
        case YAML_BLOCK_ENTRY_TOKEN:          puts("<b>Start Block (Entry)</b>");    break;
        case YAML_BLOCK_END_TOKEN:            puts("<b>End block</b>");              break;
        /* Data */
        case YAML_BLOCK_MAPPING_START_TOKEN:  puts("[Block mapping]");            break;
        case YAML_SCALAR_TOKEN:  printf("scalar %s \n", token.data.scalar.value); break;
        case YAML_FLOW_SEQUENCE_START_TOKEN:  puts("<b>Start Flow (Sequence)</b>"); break;
        case YAML_FLOW_ENTRY_TOKEN:          puts("<b>Start Flow (Entry)</b>");    break;
        case YAML_FLOW_SEQUENCE_END_TOKEN:  puts("<b>End Flow</b>"); break;
        /* Others */
        default:
          printf("Got token of type %d\n", token.type);
        }
        if(token.type != YAML_STREAM_END_TOKEN)
          yaml_token_delete(&token);
      } while(token.type != YAML_STREAM_END_TOKEN);
      yaml_token_delete(&token);
      /* END new code */

    /*cleanup */
    yaml_parser_delete(&parser);
    fclose(fh);
    return 0;
}

