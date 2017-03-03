#!/usr/bin/perl

#
# To automate running on all puzzles in 'hard.txt'....
# ./solve.pl hard.txt
#

use strict;

my $SOLVER="solvesudoku";
die "no $SOLVER found!" unless -e $SOLVER;

my $PUZZLES="simple.txt";
#my $PUZZLES="hard.txt";

$PUZZLES = $ARGV[0] if @ARGV >= 1;

open(my $fh, $PUZZLES) or die "$!\n";
my $n = 1;
while (<$fh>) {
    chomp;
    next unless /^[\.1-9]{81}$/;
    print "puzzle $n...\n";
    my $CMD = "time echo '$_' | ./$SOLVER";
    # print "$CMD\n";
    system("$CMD") == 0 or die "$!\n";
    $n++;
}
close $fh;
