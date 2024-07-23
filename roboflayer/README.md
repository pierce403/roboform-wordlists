# #OpRoboFlayer : RoboForm + Brainflayer

tl;dr: No funds at risk (yay!)

Lets see if any of the roboform lists open any brainwallets. It is unlikely to find any holding funds, but it might give some interesting insight about if and when these techniques were discovered and exploited by criminals.

## Brainflayer

The primary work here was done by Ryan in their Brainflayer project.

https://github.com/ryancdotorg/brainflayer

Ryan's DEFCON talk: https://www.youtube.com/watch?v=foil0hzl4Pg

Great walkthrough for converting address lists to bloom filters for Brainflayer:

https://www.youtube.com/watch?v=Sj3V2mjaWpQ

## Create bloom filter file
```
$ cut -d$'\t' -f1 blockchair_bitcoin_addresses_and_balance_LATEST.tsv | grep '^1' > btc_funded.txt
$ ./bitcoin-tool --batch --input-type address --output-type public-key-rmd --input-format base58check --network bitcoin --output-format hex --input-file ../btc_funded.txt > ../btc_funded.hex
$ ./hex2blf ../btc_funded.hex ../btc_funded.blf
```

## Spot check Brainflayer to ensure it's working
```
$ ~/brainflayer/brainflayer -v -b ~/btc_funded.blf -i ~/rockyou.txt -o something-rock.txt
 rate: 180228.58 p/s found:     1/28688782   elapsed:  159.180 s
$ cat something-rock.txt
374e5f09cfc1e8b9fe58b2d34242f205c8e34691:u:sha256:iloveeverything
```
Haha, so looks like there's some money in the "iloveeverything" brainwallet.

`163S3t3mHyKFTL7EhjUEWfVg3tePNNKQdS|iloveeverything|5KjviYPZvM7v1ZwJdYtLGoR4SRYXvfFExc3oHJXwdZkwoaK2oF4|0.0000117`

https://www.blockchain.com/explorer/addresses/btc/163S3t3mHyKFTL7EhjUEWfVg3tePNNKQdS

Looks like someone (not me) moved them on July 14th. They moved 79 cents with a fee of 60 cents, pocketing a sweet 19 cents. Good for them.

## Using the ps1 script to generate a bunch of wordlists

Set-ExecutionPolicy RemoteSigned -Scope CurrentUser -Force; .\generate.ps1

Generating about 60k words per second per CPU core

Generated wordlists for 2012-2016 inclusive for lengths of 8(default), 10, 12, 14, 16, 18, and 20, using uld and ulds (upper, lower, digits, specials)

Ran the wordlists through brainflayer as shown above, with no hits. Releasing wordlists so others can use them to try more localized recovery efforts (decrypting personal wallets etc).

## Download Wordlists

https://mega.nz/folder/GYUhQbpY#0V4dPnl-BBlmCVeIkLN2bQ
