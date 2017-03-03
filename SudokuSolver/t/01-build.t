#!/usr/bin/env perl

use strict;
use warnings;
use utf8;
use Test::More tests => 4;

my $TARGET = "solvesudoku";
my @SRC = ("solvesudoku.cpp");

ok(-f "Makefile", "Makefile found");
foreach my $SRC (@SRC) {
    ok(-f "$SRC", "$SRC found");
}

my $CXXFLAGS="-Wall -pedantic";

my $compiler_output = `env CXXFLAGS="$CXXFLAGS" make -B $TARGET >/dev/null 2>/dev/null`;
ok((!$? and -f "$TARGET"), "$TARGET built");

$compiler_output = `env CXXFLAGS="$CXXFLAGS -Werror" make -B $TARGET 2>&1`;
ok((!$? and -f "$TARGET"), "$TARGET built without warnings");
diag "Compiler output:\n", $compiler_output =~ s/^/  /mrg if $?;

$compiler_output = `env CXXFLAGS="$CXXFLAGS" SPEED=1 make -B $TARGET`;
