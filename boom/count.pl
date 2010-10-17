#!/usr/bin/perl

$tot = 0;
while (<>) {
	next unless /\bT=(.+?)\b/;
	$tot++;
	$c = $1;
	$cat{$c}++;
	/^\s*(\S+)\s/;
	$man{$c}{$1}++;
}

print "Total: $tot\n";
for $c (sort keys %cat) {
	print "    $c: $cat{$c}\n";
	for $m (sort keys %{ $man{$c} }) {
		print "\t$m: $man{$c}{$m}\n";
	}
}
