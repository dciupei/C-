#!/usr/bin/env perl

use strict;
use warnings;
use utf8;
use Test::More tests => 5;
 
 
my @labels = ("Empty input", "Square input", "Rectangular input", "Ragged input", "Big input");
my @inputs = ("", "123\n456\n789", "ab\ncd\nef\ngh", "abc\nd\nefg", "a\n"x80);
my @outputs = ("", "741\n852\n963", "geca\nhfdb", "eda\nf b\ng c", "a"x80);

for (my $i = 0; $i < @inputs; ++$i) {
	my $label = $labels[$i];
	my $input = $inputs[$i];
	my $output = $outputs[$i];
	chomp $input;
	chomp $output;

	my $result = `echo \"$input\" | ./rot90`;
	chomp $result;

	if (!ok($output eq $result, $label)) {
		# Let's not print the big one out
		if ($label ne "Big input") {
			diag("input:\n", ($input =~ s/^/  /mrg), "\nexpected:\n", ($output =~ s/^/  /mrg), "\ngot:\n", ($result =~ s/^/  /mrg));
		}
	}
}
