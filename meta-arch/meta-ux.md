# Meta UI/UX

## User journey's 

_not yet written_

Questions to resolve

##	Manual pull or auto pull.

Backend load is a concern - the more random te requests come in, the better. They should be updated at least once every 24 hours (H1) and it is makes no sense to do it more often that (H2).

So IMHO best practice would be auto pull with a nice bit random delay; and ONLY allow a reload by the user if the following two conditions are true a) the last successful load is more than H2 ago and b) the last successfull or unsuccessful reload is more than H3 ago.


##	Can a user disable tracing.

Given the Waag and EU 'toolbox' requirements - entirely up to the user.

##	Does the user need backups, etc.

The current protocol does not require backups on the phone side by design. Post contamination divulge/update of the backend the key is forcefully re-seeded. And therefore the `phone is as new'.

## Process for the infected / divulge process

Given the Waag and EU 'toolbox' requirements - it should be assumed that this can only be done by a medical professional who is licensed to practice. And who will also give the appropriate advice.

The actual process is TBD - but given that a low common denominator is desired - it may be something akin to providing a 6+4 digit number on the phone which is valid for 30 minutes - that the health professional enters into a web interface on his or her device/desktop. 

The actual protocol from thereon is not defined - but probably entails the phone posting a nonce along the 6 digits to some queue; it polling/WS-getting that nonce, with the 4 digits back signed by a medical professional. After which the phone posts its seeds, destroys its old data, and restarts the seed process.

The user is shownsome sort of progress, OK thing.


--------


H1:		daily guaranteed update rate - e.g 24 Hrs

H2: 	target update rate / best effort	- e.g. 6 hours.

H3:		max repeat of retries we can handle: 120 seconds
