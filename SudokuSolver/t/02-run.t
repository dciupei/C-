#!/usr/bin/env perl

use strict;
use warnings;
use utf8;
use Test::More tests => 11;

my $SOLVER="./solvesudoku";
my $CHECKER="./sudokucheck.pl";

ok(-e "$SOLVER", "$SOLVER exists");

my $PUZZLES="testpuzzles.txt";
$PUZZLES = $ARGV[0] if @ARGV >= 1;
open(my $fh, $PUZZLES) or die "$!\n";

while (<$fh>) {
    chomp;
    next unless /^[\.1-9]{81}$/;

    my $output = `echo '$_' | $SOLVER | $CHECKER`;
    if (!ok(!$?, "$_")) {
      diag($output =~ s/^/  /mrg);
    }
}

close $fh;
