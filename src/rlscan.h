/*
 *  Copyright 2007 Adrian Thurston <thurston@complang.org>
 */

#ifndef _RLSCAN_H
#define _RLSCAN_H

#include <iostream>
#include "rlscan.h"
#include "vector.h"
#include "rlparse.h"
#include "parsedata.h"
#include "avltree.h"
#include "vector.h"

using std::istream;
using std::ostream;

extern char *Parser_lelNames[];

struct Scanner
{
	Scanner( InputData &id, const char *fileName, istream &input,
			Parser6 *inclToParser, char *inclSectionTarg,
			int includeDepth, bool importMachines )
	: 
		id(id), fileName(fileName), 
		input(input),
		inclToParser(inclToParser),
		inclSectionTarg(inclSectionTarg),
		includeDepth(includeDepth),
		importMachines(importMachines),
		cur_token(0),
		line(1), column(1), lastnl(0), 
		parser(0), ignoreSection(false), 
		parserExistsError(false),
		whitespaceOn(true),
		lastToken(0)
		{}

	bool duplicateInclude( char *inclFileName, char *inclSectionName );

	/* Make a list of places to look for an included file. */
	char **makeIncludePathChecks( const char *curFileName, const char *fileName, int len );
	std::ifstream *tryOpenInclude( char **pathChecks, long &found );

	void handleMachine();
	void handleInclude();
	void handleImport();

	void init();
	void token( int type, char *start, char *end );
	void token( int type, char c );
	void token( int type );
	void processToken( int type, char *tokdata, int toklen );
	void directToParser( Parser6 *toParser, const char *tokFileName, int tokLine, 
		int tokColumn, int type, char *tokdata, int toklen );
	void flushImport( );
	void importToken( int type, char *start, char *end );
	void pass( int token, char *start, char *end );
	void pass();
	void updateCol();
	void startSection();
	void endSection();
	void do_scan();
	bool active();
	ostream &scan_error();

	InputData &id;
	const char *fileName;
	istream &input;
	Parser6 *inclToParser;
	char *inclSectionTarg;
	int includeDepth;
	bool importMachines;

	/* For import parsing. */
	int tok_cs, tok_act;
	int *tok_ts, *tok_te;
	int cur_token;
	static const int max_tokens = 32;
	int token_data[max_tokens];
	char *token_strings[max_tokens];
	int token_lens[max_tokens];

	/* For section processing. */
	int cs;
	char *word, *lit;
	int word_len, lit_len;

	/* For character scanning. */
	int line;
	InputLoc sectionLoc;
	char *ts, *te;
	int column;
	char *lastnl;

	/* Set by machine statements, these persist from section to section
	 * allowing for unnamed sections. */
	Parser6 *parser;
	bool ignoreSection;

	/* This is set if ragel has already emitted an error stating that
	 * no section name has been seen and thus no parser exists. */
	bool parserExistsError;

	/* This is for inline code. By default it is on. It goes off for
	 * statements and values in inline blocks which are parsed. */
	bool whitespaceOn;

	/* Keeps a record of the previous token sent to the section parser. */
	int lastToken;
};

#endif