/* Author: Steffen Viken Valvaag <steffenv@cs.uit.no> */
#include "list.h"
#include "set.h"
#include "common.h"
#include "printing.h"

/*
 * Case-insensitive comparison function for strings.
 */
static int compare_words(void *a, void *b)
{
    return strcasecmp(a, b);
}

/*
 * Returns the set of (unique) words found in the given file.
 */
static set_t *tokenize(char *filename)
{
	set_t *wordset = set_create(compare_words);
	list_t *wordlist = list_create(compare_words);
	list_iter_t *it;
	FILE *f;
	printf("Trying to open %s\n", filename);
	f = fopen(filename, "r");
	if (f == NULL) {
		perror("fopen");
		ERROR_PRINT("fopen() failed");
		exit(1);
	}
	tokenize_file(f, wordlist);
	
	it = list_createiter(wordlist);
	while (list_hasnext(it)) {
		set_add(wordset, list_next(it));		
	}
	list_destroyiter(it);
	list_destroy(wordlist);
	return wordset;
}

/*
 * Prints a set of words.
 */
static void printwords(char *prefix, set_t *words)
{
	set_iter_t *it;
	
	it = set_createiter(words);
	INFO_PRINT("%s: ", prefix);
	while (set_hasnext(it)) {
		INFO_PRINT(" %s", (char *)set_next(it));
	}
	printf("\n");
	set_destroyiter(it);
}

/*
 * Main entry point.
 */
int main(int argc, char **argv)
{
	char *spamdir, *nonspamdir, *maildir;
	if (argc != 4) {
		DEBUG_PRINT("usage: %s <spamdir> <nonspamdir> <maildir>\n", argv[0]);
		return 1;
	}
	spamdir = argv[1];
	nonspamdir = argv[2];
	maildir = argv[3];

	// feilsøking 
	FILE *f = fopen("data/spam/spam1.txt", "r");
	if(f == NULL) {
		printf("CAN NOT OPEN\n");
		exit(1);
	}

	// Creates a list containing the filnames of all the files in the directory 
	list_t *all_mail = find_files(maildir); 
	list_t *all_spam = find_files(spamdir); 
	list_t *all_nonspam = find_files(nonspamdir); 

	// Make a iter for each list
	void *mail_iter = list_createiter(all_mail); 
	void *spam_iter = list_createiter(all_spam); 
	void *nonspam_iter = list_createiter(all_nonspam); 

	// Use tokenize to make a set of unique words found in the given file, and use the iter 
	// to iterate through all the spamfiles 
	set_t *spam_set, *next_spam; 
	spam_set = tokenize(list_next(spam_iter)); 

	// Use a while loop to iterate through all the sets and find the intersection 
	while (list_hasnext(spam_iter))
	{
		next_spam = tokenize(list_next(spam_iter)); // make a new set, to store the next set 
		spam_set = set_intersection(spam_set, next_spam); 
	}

	// Use tokenize to make a set of unique words found in the given file, and use the iter 
	// to iterate through all the nonspamfiles 
	set_t *nonspam_set, *next_nonspam;
	nonspam_set= tokenize(list_next(nonspam_iter));

	// Use a while loop to iterate through all the sets and find the union 
	while (list_hasnext(nonspam_iter))
	{
		next_nonspam = tokenize(list_next(nonspam_iter)); // make a new set, to store the next set 
		nonspam_set = set_union(nonspam_set, next_nonspam); 
	}

	// Find the spamwords by finding the words that is found in spam, but not in nonspam 
	set_t *spam_mail, *mail_set; 
	spam_mail = set_difference(spam_set, nonspam_set);

	// Creates a while loop that iterates through all the mail as long as there are several mails left in the directory
	while(list_hasnext(mail_iter))
	{
		void *name = list_next(mail_iter); // do it in two operations to get the name of the file that are printing 
		mail_set = tokenize(name); 
		mail_set = set_intersection(mail_set, spam_mail);

		// Checks if the email contains spam words. If yes, return that it is spam, 
		// otherwise return that it is not spam.
		if (set_size(mail_set) != 0)
		{
			printf("% s, %d spam word(s) -> Spam \n\n", name,  set_size(mail_set));
		}
		else
		{
			printf("%s: %d spam word(s) -> No spam \n\n", name, set_size(mail_set));
		}
	}

    return 0;
}
