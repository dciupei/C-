#!/usr/bin/env perl

use strict;
use warnings;
use utf8;
use Test::More tests => 3;

ok(-f "README", "README found");

# I've decided I hate Perl, so let's just grep
ok(!system("grep -qi \"name:.*[[:alpha:]]\" README"), "README contains a name");
ok(!system("grep -qi \"email.*@.*\" README"), "README contains an email");

# Do we really want to require a SID in the README?
# These repos are supposed to be private, but the students might screw it up...
# ok($readme =~ qr/^sid:\s+[0-9]+$/mi, "README contains a SID");
