#!/usr/bin/perl

$BASE = "http://search.digikey.com/scripts/DkSearch/dksus.dll";
$QMOD = "stock=1&pbfree=1&rohs=1";
$URL = "$BASE?$QMOD&k=";

$DEBUG = 0;


sub query
{
	local @q = ();

	open(PIPE, "wget -O - '$_[0]' |") || die "wget: $!";
	while (<PIPE>) {
		chop;
		push(@q, $_);
	}
	close PIPE;
	return @q;
}


# --- Read query specification ------------------------------------------------


if ($ARGV[0] eq "-t") {
	$test = 1;
	shift @ARGV;
}

$spec = $ARGV[0];

while (<>) {
	s/#.*//;
	next if /^\s*$/;
	chop;
	push(@q, $_);
}

$id = shift @q;
$key = shift @q;
$cat = shift @q;
for (@q) {
	die "not a field=value pair: \"$_\"" unless /\s*=\s*/;
	push(@{ $f{$`} }, $');
}


# --- Select category ---------------------------------------------------------


$url = $URL.$key;

@q = &query($url);

if ($q[1] =~ /<title>Digi-Key</) {
	undef $found;
	for (@q) {
		next unless /Cat=(\d+)[&"].*?>\s*([^,]*?)\s*</;
		next if $2 ne $cat;
		$found = $1;
		last;
	}

	die "category \"$cat\" not found" unless defined $found;


# --- Get parameter tables ----------------------------------------------------


	$url .= "&Cat=$found";

	@q = &query($url);
}

for (@q) {
	if (/^<th>\s*(.*?)\s*<\/th>/) {
		print STDERR "col name \"$1\"\n" if $DEBUG;
		push(@col_name, $1);
		next;
	}
	if (/<select .* name=([^ >]+)/) {
		$cols++;
		$col = $col_name[$cols-1];
		die "cols = $cols" unless defined $col;
		print STDERR "$col -> \"$1\"\n" if $DEBUG;
		$col_field{$col} = $1;
		next;
	}
	next unless /<option value=(\d+)>\s*(.*?)\s*(<.*)?$/;
	print STDERR "val{$col}{$2} = \"$1\"\n" if $DEBUG;
	$val{$col}{$2} = $1;
}

for (keys %f) {
	$field = $col_field{$_};
	die "no such field: $_" unless defined $field;
	for $v (@{ $f{$_} }) {
		$value = $val{$_}{$v};
		die "no such value: \"$_\"=\"$f{$_}\"" unless defined $value;
		$url .= "&$field=$value";
	}
}


# --- Stop here if in test mode -----------------------------------------------


if ($test) {
	print "$url\n";
	exit(0);
}


# --- Output file header ------------------------------------------------------


print "#EQU\n# Generated from $spec\n# ".`date`."\n";


# --- Get the pages -----------------------------------------------------------


$url =~ s/\?/?Selection\&/;	# magic key to the pages
$page = 1;

while (1) {
	@q = &query("$url&page=$page");
	$more = 0;
	for (@q) {
		$more = 1 if />Next</;
		next unless /-ND">\s*([^>]*-ND)\s*<\/a><\/td><td>\s*(.*?)\s*</;
		print "DIGI-KEY $1\t $id $2\n";
	}
	last unless $more;
	$page++;
}
