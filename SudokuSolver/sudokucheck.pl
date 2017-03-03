#!/usr/bin/perl

#
# Check Soduku puzzle solution.
#
# Uses the last 9 lines read from stdin that contain 9 exactly digits
# to form the (potential) solution to a 9x9 Sudoku Puzzle.
# Checks that each row, col, and 3x3 block contain all the digits
# from 1 to 9 exactly once.
#
# If correct solution then it prints the following to stdout
#   rows ok!
#   columns ok!
#   blocks ok!
# else you will get a FAIL: message with a description of the fail.
#

use strict;

my $PRINTSOLUTION = 0;

#
# harvest array of digits from input string
#
sub digits {
    my $str = shift;
    my @digits;
    while (/(\d)/g) {
	my $digit = $1;
	push @digits, $digit;
    }
    return @digits;
}

#
# Get array of all rows of 9 digits
#
my @alldigits;  
for (<>) {
    my @digits = digits($_);
    next unless @digits == 9;
    push @alldigits, \@digits;
}
    
die "9 rows of digits not found!\n" unless @alldigits >= 9;

#
# Get last eight rows of digits
# which should form solution to puzzle
#
my @puzzle;
for (my $i = 1; $i <= 9; $i++) {
    my $row = @alldigits[-$i];
    $puzzle[9 - $i] = $row;
}

#
# Fetch puzzle entry at row=r column=c;
#
sub puzzle {
    my ($r,$c) = @_;
    my $row = $puzzle[$r];
    return $row->[$c];
}

#
# Print solution.
#
if ($PRINTSOLUTION) {
    for (my $r = 0; $r < 9; $r++) {
	for (my $c = 0; $c < 9; $c++) {
	    my $d = puzzle($r,$c);
	    print "$d ";
	}
	print "\n";
    }
}

sub alldigits {
    my @digits = @_;
    my @set = (0, 0, 0, 0, 0, 0, 0, 0, 0);
    foreach my $d (@digits) {
# 	die "zero!\n" if $d == 0;
# 	die "duplicate!\n" if $set[$d-1] == 1;
	return 0 if $d == 0;  # zero!
	return 0 if $set[$d-1] == 1; # duplicate!
	$set[$d-1] = 1 ;
    }
    for (my $i = 0; $i < 9; $i++) {
#	die "missing $i!\n" unless $set[$i] == 1;
	return 0 unless $set[$i] == 1;  # missing digit!
    }
    return 1; # good
}

#
# Check rows
#
for (my $r = 0; $r < 9; $r++) {
    my @digits;
    for (my $c = 0; $c < 9; $c++) {
	push @digits, puzzle($r,$c);
    }
    die "FAIL: bogus row $r!\n" unless alldigits(@digits);
}

print "rows ok!\n";

#
# Check columns.
#
for (my $c = 0; $c < 9; $c++) {
    my @digits;
    for (my $r = 0; $r < 9; $r++) {
	push @digits, puzzle($r,$c);
    }
    die "FAIL: bogus column $c!\n" unless alldigits(@digits);
}

print "columns ok!\n";

#
# Check 3x3 blocks;
#
for (my $R = 0; $R < 9; $R += 3) {
    for (my $C = 0; $C < 9; $C += 3) {
	my @digits;
	for (my $i = 0; $i < 3; $i++) {
	    for (my $j = 0; $j < 3; $j++) {
		push @digits, puzzle($R + $i, $C + $j);
	    }
	}
	die "FAIL: bogus block $R,$C!\n" unless alldigits(@digits);
    }
}

print "blocks ok!\n";

exit 0;  # all good
