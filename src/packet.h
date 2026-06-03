#ifndef PACKET_H
#define PACKET_H

#include <cstdlib>
#include <vector>
#include <array>
#include <string>
#include <queue>
#include <utility>
#include <iostream>
#include <map>
#include <random>

using namespace std;

inline constexpr unsigned int seed = 12345; 
inline constexpr int NUMBER_OF_PRIORITIZED_DOMAINS = 99; //How many domains have their unique Priority level
inline constexpr int LIKELIHOOD_OF_PRIORIIZED_DOMAINS = 20; //in percentage (%). AKA how likely it is for a packet to be headed to a prioritzed domain
inline constexpr int NUMBER_OF_APQ_LEVELS = 5; //How many domains from 
inline constexpr int DOMAINS_PER_APQ_LEVEL = 20;
inline constexpr int UNPRIORITIZED = NUMBER_OF_PRIORITIZED_DOMAINS;

//Number of packets in the testCase
inline constexpr int NUMBER_OF_PACKETS = 100;
//Maximum time the simulation could run for
inline constexpr int MAX_SIM_TIME = 1000000000;
//Maximum time between times the Router receives messages
inline constexpr int MAX_RECEIVING_GAP = 10;
//Maximum time the Router may take to process messages
//The higher ther Max processing time, the more packets get caught in a jam and the less the Normal queue's speed advantage matters
inline constexpr int MAX_PROCESSING_TIME = 50;

//Array of websites sorted by their priority rank
inline array<string, NUMBER_OF_PRIORITIZED_DOMAINS + 1> g_dpl_arr 
{
    "shopee.vn", 
    "ssi.com.vn",
    "amazon.com", 
    "bestbuy.com", 
    "etsy.com", 
    "ebay.com",
    "zoom.us",
    "netflix.com", 
    "stackoverflow.com",
    "linkedin.com",
    "x.com",
    "github.com",
    "google.com",
    "imdb.com",
    "weather.com",
    "yahoo.com",
    "bing.com",
    "duckduckgo.com",
    "nytimes.com",
    "bbc.com",
    "cnn.com",
    "wikipedia.org",
    "apple.com",
    "walmart.com",
    "target.com",
    "youtube.com",
    "microsoft.com",
    "craigslist.org",
    "pinterest.com",
    "instagram.com",
    "facebook.com",
    "reddit.com",
    "quora.com",
    "medium.com",
    "dev.to",
    "hashnode.com",
    "gitlab.com",
    "bitbucket.org",
    "npmjs.com",
    "pypi.org",
    "docker.com",
    "kubernetes.io",
    "aws.amazon.com",
    "cloud.google.com",
    "azure.microsoft.com",
    "digitalocean.com",
    "heroku.com",
    "vercel.com",
    "netlify.com",
    "cloudflare.com",
    "godaddy.com",
    "namecheap.com",
    "bluehost.com",
    "hostinger.com",
    "siteground.com",
    "wordpress.com",
    "wordpress.org",
    "wix.com",
    "squarespace.com",
    "shopify.com",
    "magento.com",
    "bigcommerce.com",
    "woocommerce.com",
    "stripe.com",
    "paypal.com",
    "visa.com",
    "mastercard.com",
    "americanexpress.com",
    "chase.com",
    "bankofamerica.com",
    "wellsfargo.com",
    "citi.com",
    "fidelity.com",
    "vanguard.com",
    "schwab.com",
    "bloomberg.com",
    "reuters.com",
    "wsj.com",
    "ft.com", 
    "economist.com",
    "forbes.com",
    "fortune.com",
    "hbr.org",
    "inc.com",
    "entrepreneur.com",
    "fastcompany.com",
    "wired.com",
    "techcrunch.com",
    "venturebeat.com",
    "thenextweb.com",
    "engadget.com",
    "gizmodo.com",
    "theverge.com",
    "arstechnica.com",
    "slashdot.org",
    "hackernews.ycombinator.com",
    "producthunt.com",
    "kickstarter.com",
    "patreon.com",
    "UNPRIORITIZED"
};

//Mock hashtable/dictionary of 100 domains and their assigned priority rank from 0 to 98, plus priority rank 99 to represent non-prioritized domains
//Smaller rank = Higher priority
inline map<string, int> g_dpl_map
{
    { "shopee.vn", 0 },
    { "ssi.com.vn", 1 }, 
    { "amazon.com", 2 }, 
    { "bestbuy.com", 3 }, 
    { "etsy.com", 4 }, 
    { "ebay.com", 5 }, 
    { "zoom.us", 6 }, 
    { "netflix.com", 7 }, 
    { "stackoverflow.com", 8 }, 
    { "linkedin.com", 9 }, 
    { "x.com", 10 }, 
    { "github.com", 11 }, 
    { "google.com", 12 }, 
    { "imdb.com", 13 }, 
    { "weather.com", 14 }, 
    { "yahoo.com", 15 }, 
    { "bing.com", 16 }, 
    { "duckduckgo.com", 17 },
    { "nytimes.com", 18 }, 
    { "bbc.com", 19 }, 
    { "cnn.com", 20 }, 
    { "wikipedia.org", 21 },
    { "apple.com", 22 },
    { "walmart.com", 23 },
    { "target.com", 24 }, 
    { "youtube.com", 25 }, 
    { "microsoft.com", 26 }, 
    { "craigslist.org", 27 }, 
    { "pinterest.com", 28 }, 
    { "instagram.com", 29 }, 
    { "facebook.com", 30 }, 
    { "reddit.com", 31 }, 
    { "quora.com", 32 }, 
    { "medium.com", 33 }, 
    { "dev.to", 34 }, 
    { "hashnode.com", 35 }, 
    { "gitlab.com", 36 }, 
    { "bitbucket.org", 37 }, 
    { "npmjs.com", 38 }, 
    { "pypi.org", 39 }, 
    { "docker.com", 40 }, 
    { "kubernetes.io", 41 }, 
    { "aws.amazon.com", 42 }, 
    { "cloud.google.com", 43 }, 
    { "azure.microsoft.com", 44 }, 
    { "digitalocean.com", 45 }, 
    { "heroku.com", 46 }, 
    { "vercel.com", 47 }, 
    { "netlify.com", 48 }, 
    { "cloudflare.com", 49 }, 
    { "godaddy.com", 50 }, 
    { "namecheap.com", 51 }, 
    { "bluehost.com", 52 }, 
    { "hostinger.com", 53 }, 
    { "siteground.com", 54 }, 
    { "wordpress.com", 55 }, 
    { "wordpress.org", 56 }, 
    { "wix.com", 57 },
    { "squarespace.com", 58 }, 
    { "shopify.com", 59 }, 
    { "magento.com", 60 }, 
    { "bigcommerce.com", 61 }, 
    { "woocommerce.com", 62 }, 
    { "stripe.com", 63 }, 
    { "paypal.com", 64 }, 
    { "visa.com", 65 }, 
    { "mastercard.com", 66 }, 
    { "americanexpress.com", 67 }, 
    { "chase.com", 68 }, 
    { "bankofamerica.com", 69 }, 
    { "wellsfargo.com", 70 }, 
    { "citi.com", 71 }, 
    { "fidelity.com", 72 }, 
    { "vanguard.com", 73 }, 
    { "schwab.com", 74 }, 
    { "bloomberg.com", 75 }, 
    { "reuters.com", 76 }, 
    { "wsj.com", 77 }, 
    { "ft.com", 78 }, 
    { "economist.com", 79 }, 
    { "forbes.com", 80 }, 
    { "fortune.com", 81 }, 
    { "hbr.org", 82 }, 
    { "inc.com", 83 }, 
    { "entrepreneur.com", 84 }, 
    { "fastcompany.com", 85 }, 
    { "wired.com", 86 }, 
    { "techcrunch.com", 87 }, 
    { "venturebeat.com", 88 }, 
    { "thenextweb.com", 89 }, 
    { "engadget.com", 90 }, 
    { "gizmodo.com", 91 }, 
    { "theverge.com", 92 }, 
    { "arstechnica.com", 93 }, 
    { "slashdot.org", 94 }, 
    { "hackernews.ycombinator.com", 95 }, 
    { "producthunt.com", 96 }, 
    { "kickstarter.com", 97 }, 
    { "patreon.com", 98 }, 
    { "UNPRIORITIZED", 99 }
};


class Packet
{
private:
    //The domain the packet is headed to
    string m_domain;

    int m_receivingTimestamp;

    //Simulate how long the packet takes to send
    //In this codebase, waitTime shall be added to a variable instead of being how long the simulation program has to wait
    int m_processingTime;

public:
    Packet();

    string getDomain() {return m_domain;}

    int getPriorityLevel();

    int getReceivingTimestamp() {return m_receivingTimestamp;}
    
    int getProcessingTime() {return m_processingTime;}

    static void generateTestCase(queue<Packet>& q);

    static void printTestCase(queue<Packet> q);
};

#endif