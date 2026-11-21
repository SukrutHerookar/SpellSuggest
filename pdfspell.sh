#!/usr/bin/env bash
#
# pdfspell.sh - extract text from PDF files with pdftotext and check it with codespell.
#
# Usage:
#   ./pdfspell.sh file.pdf [more.pdf ...]
#   ./pdfspell.sh -k file.pdf              # keep the extracted .txt next to the PDF
#   ./pdfspell.sh -i words.txt cv.pdf      # ignore words listed in words.txt (one per line)
#   ./pdfspell.sh -o misspelled.txt cv.pdf # also write the misspelled words to a file
#
# The -o file has one misspelled word per line, sorted, without duplicates,
# covering all PDFs checked. It is always (re)written, and is empty when no
# mistakes are found, so a program reading it never sees stale results.
#
# Exit code: 0 = no mistakes found, 1 = mistakes found, 2 = usage/dependency error.

set -euo pipefail

keep_txt=false
ignore_file=""
out_file=""

usage() {
    echo "Usage: $(basename "$0") [-k] [-i ignore-words-file] [-o output-file] file.pdf [more.pdf ...]" >&2
    echo "  -k   keep the extracted text file (file.txt next to file.pdf)" >&2
    echo "  -i   file with words codespell should ignore, one per line" >&2
    echo "  -o   write the misspelled words to this file, one per line" >&2
    exit 2
}

while getopts ":ki:o:h" opt; do
    case "$opt" in
        k) keep_txt=true ;;
        i) ignore_file="$OPTARG" ;;
        o) out_file="$OPTARG" ;;
        h) usage ;;
        \?) echo "Unknown option: -$OPTARG" >&2; usage ;;
        :)  echo "Option -$OPTARG needs an argument" >&2; usage ;;
    esac
done
shift $((OPTIND - 1))

[[ $# -ge 1 ]] || usage

# Check dependencies
for cmd in pdftotext codespell; do
    if ! command -v "$cmd" >/dev/null 2>&1; then
        echo "Error: '$cmd' is not installed." >&2
        case "$cmd" in
            pdftotext) echo "  Install it with: sudo apt install poppler-utils" >&2 ;;
            codespell) echo "  Install it with: sudo apt install codespell   (or: pipx install codespell)" >&2 ;;
        esac
        exit 2
    fi
done

if [[ -n "$ignore_file" && ! -f "$ignore_file" ]]; then
    echo "Error: ignore-words file '$ignore_file' not found." >&2
    exit 2
fi

# Make sure the output file can be written before doing any work
if [[ -n "$out_file" ]]; then
    if ! : > "$out_file" 2>/dev/null; then
        echo "Error: cannot write to output file '$out_file'." >&2
        exit 2
    fi
fi

codespell_args=()
[[ -n "$ignore_file" ]] && codespell_args+=(--ignore-words "$ignore_file")

found_errors=false
words_tmp="$(mktemp)"   # misspelled words from all PDFs, before sorting
txt=""
cleanup() {
    rm -f "$words_tmp"
    if ! $keep_txt && [[ -n "$txt" ]]; then rm -f "$txt"; fi
}
trap cleanup EXIT

for pdf in "$@"; do
    if [[ ! -f "$pdf" ]]; then
        echo "Skipping '$pdf': file not found." >&2
        found_errors=true
        continue
    fi

    if $keep_txt; then
        txt="${pdf%.*}.txt"
    else
        txt="$(mktemp --suffix=.txt)"
    fi

    echo "==> $pdf"

    # -layout keeps the page layout so line numbers roughly match what you see
    if ! pdftotext -layout "$pdf" "$txt" 2>/dev/null; then
        echo "    Could not extract text (is the PDF encrypted or damaged?)." >&2
        found_errors=true
        continue
    fi

    if [[ ! -s "$txt" ]] || ! grep -q '[[:alpha:]]' "$txt"; then
        echo "    No text found - the PDF is probably a scanned image (needs OCR first)." >&2
        continue
    fi

    # codespell exits non-zero when it finds mistakes.
    # Its lines look like:  /path/file.txt:12: recieve ==> receive
    if output="$(codespell "${codespell_args[@]}" "$txt" 2>&1)"; then
        echo "    No spelling mistakes found."
    else
        found_errors=true
        # Show the PDF's name instead of the temp file path
        echo "$output" | sed "s|^$txt|    $(basename "$pdf")|"
        # Keep just the misspelled word from each line
        echo "$output" | sed -nE 's/^.*:[0-9]+: (.+) ==> .*$/\1/p' >> "$words_tmp"
    fi

    $keep_txt && echo "    Text saved to $txt"

    if ! $keep_txt; then
        rm -f "$txt"
    fi
done

if [[ -n "$out_file" ]]; then
    sort -u "$words_tmp" > "$out_file"
    count=$(wc -l < "$out_file")
    echo "Wrote $count misspelled word(s) to $out_file"
fi

$found_errors && exit 1
exit 0
