# FAQ: Decentralized Proximity Tracing

This FAQ attempts to answer frequently asked questions about the DP-3T project, the problems it tries to address, and its design choices. It is by no means complete. We’ll be updating this FAQ as we go, for now we have been focussing on answering the technical questions first. Feedback is very welcome.

 * [Protocol Questions](#protocol-questions)
   * [P1: Why don’t infected users upload the ephemeral Bluetooth identifiers (EphIDs) they have observed to the backend server, so that other apps can download them and check for contacts locally?](#p1-why-dont-infected-users-upload-the-ephemeral-bluetooth-identifiers-ephids-they-have-observed-to-the-backend-server-so-that-other-apps-can-download-them-and-check-for-contacts-locally)
   * [P2: Why don’t infected users upload the ephemeral Bluetooth identifiers (EphIDs) they have observed to the backend server, so that other apps can ask the server if there is a match with their own EphIDs?](#p2-why-dont-infected-users-upload-the-ephemeral-bluetooth-identifiers-ephids-they-have-observed-to-the-backend-server-so-that-other-apps-can-ask-the-server-if-there-is-a-match-with-their-own-ephids)
   * [P3: Why not use multi party computation or custom privacy\-preserving protocols (PSI, PIR, etc\.) instead to query the server for the observed ephemeral Bluetooth identifiers?](#p3-why-not-use-multi-party-computation-or-custom-privacy-preserving-protocols-psi-pir-etc-instead-to-query-the-server-for-the-observed-ephemeral-bluetooth-identifiers)
   * [P4: Why is the system not using public key cryptography when broadcasting identifiers?](#p4-why-is-the-system-not-using-public-key-cryptography-when-broadcasting-identifiers)
   * [P5: Why not use mixnets or other anonymous communication systems to query the server?](#p5-why-not-use-mixnets-or-other-anonymous-communication-systems-to-query-the-server)
   * [P6: Why do infected people upload a seed (which enables recreating EphIDs) instead of their individual EphIDs?](#p6-why-do-infected-people-upload-a-seed-which-enables-recreating-ephids-instead-of-their-individual-ephids-)
   * [P7: Why do you call your design "decentralized" while having a backend?](#p7-why-do-you-call-your-design-decentralized-while-having-a-backend)

## Protocol Questions

Questions regarding the underlying protocol and mitigations for known vulnerabilities

### P1: Why don’t infected users upload the ephemeral Bluetooth identifiers (`EphIDs`) they have observed to the backend server, so that other apps can download them and check for contacts locally?

*Short answer:** The bandwidth cost of downloading all observed Bluetooth
identifiers from all infected individuals is high. Furthermore, it facilitates
attacks that insert or remove contact events. Finally, it reveals interactions
between pseudonymous users to the backend server, without providing extra
privacy in comparison with publishing the infected users’ seeds.

*Long answer:** It is possible to build a privacy-friendly contact tracing
system by letting diagnosed patients upload the list of observed ephemeral
Bluetooth identifiers (EphIDs). All other smartphones would then download this list,
and check if any of the identifiers they generated was seen by (and therefore in
close physical proximity to) an infected patient.

This option, however, is very costly. In Europe there are more than 30,000
patients a day. The number of observed EphIDsis also high. We expect people to
be in close physical proximity with many people. For instance, spending 24 hours
at home with your partner will already yield 96 recorded EphIDs(assuming they
change every 15 minutes). So let’s say an infected person uploads 5000 unique
contact events for 21 days. We then need to transfer 150 million records. Even
using efficient representations (e.g., a cuckoo filter) this would take at least
600MB to be downloaded by every app, every day.

Sending observed contacts also increases the likelihood that a tech-savvy user
creates fake contact events, which in turn can lead to unnecessary anxiety. To
fake at-risk contacts, an infected user simply inserts additional EBIDs from
other users to his local storage. In DP3T, in which an infected user shares
their own EBIDs, the barrier to fake contact events is much higher. The infected
user would have to actively broadcast their own EBIDs via another device to fake
contacts with other users.


### P2: Why don’t infected users upload the ephemeral Bluetooth identifiers (`EphIDs`) they have observed to the backend server, so that other apps can ask the server if there is a match with their own `EphIDs`?

*Short answer:* This results in a high load on the server and either reveals
privacy sensitive information to the server, or requires anonymous
communication.

*Long answer:** In this solution, rather than apps downloading a list of all
EphIDs observed by infected patients, they would instead query the backend
server with their own EphIDs to ask if any of them has been in contact with an
infected patient. The consequence is a significant increase in bandwidth usage.
In particular, the apps must daily query all the EphIDs that they broadcasted
in the last 21 days (as newly diagnosed patients might have seen these in the
past), which is estimated as approximately 2,000 EphIDs per day per user.

For privacy reasons, it is essential that the server cannot link all EphIDs of a
single user. Therefore, users must query their EphIDs separately and via an
anonymous communication network so that their identifiers remain unlinkable. For
50 million users, the server must therefore be able to process more than a
million lookup queries per second.

### P3: Why not use multi party computation or custom privacy-preserving protocols (PSI, PIR, etc.) instead to query the server for the observed ephemeral Bluetooth identifiers?

We all love privacy-preserving cryptography. However, the scale at which this
system must operate is significant: a server set size of 150 million entries of
16 bytes each (corresponding to 30k new infections a day and 5000 distinct
recorded EphIDs), a client set of 2,000 items, and 50 million daily queries
(>500 queries per second).

It might be possible to design and deploy special purpose cryptographic
techniques that scale to this level and we are aware of research prototypes that
might be able to fulfil the requirements and for which code might be available.
However, a significant investment of time and engineering effort would still be
needed to take such prototypes and develop them to the point where they could be
deployed in a mobile application.

### P4: Why is the system not using public key cryptography when broadcasting identifiers?

In DP-3T any device must communicate with all of their neighbours, meaning that
authentication is impossible. Thus, a malicious party can inject their own
traffic and hence participate in any exchange.

Secondly, any application of public key cryptography would require a connection
between devices or multiple broadcasts (each broadcast is limited to only 11
bytes and the smallest public keys are around 32 bytes). In a crowded
environment there is substantial message loss from interference between
messages. It is unlikely that performing N^2 connections or exchanges between N
apps would function effectively, in contrast to N broadcasts in the current
protocol.

### P5: Why not use mixnets or other anonymous communication systems to query the server?

Our design does not rely on anonymous communication systems to provide its
privacy properties, it only uses a small amount of dummy messages to hide
uploads to the backend and epidemiologists.

We considered using an anonymous communication system to efficiently query the server. However, we decided against doing so for the following reasons:
 1. Relying on any form of anonymous communication system increases the
    complexity of the system. Both in terms of integrating anonymous
    communication into the app, as well as the server infrastructure needed to
    support tens of millions of apps. (Even Tor, the most widely deployed
    anonymous communication network, would struggle under this load.)
 2. All anonymous communication systems must trade anonymity, latency, and
    bandwidth overhead. It is not clear what is the good choice in this
    scenario.
 3. We need to take the security properties of the anonymous communication
    system into account in our analysis. E.g., should we protect against a
    global passive adversary or not? How well does the system protect against
    intersection attacks?

### P6: Why do infected people upload a seed (which enables recreating `EphIDs`) instead of their individual EphIDs ?

This is a choice that is made purely for performance reasons. It is much more
efficient to send a single 32 byte seed than sending all EphIDs generated during
the infectious period (e.g., 21 days). We are aware that this makes the EphIDs
of infected patients linkable during the infectious period.

For comparison, sending 21 days of EphIDs rotated every 15 minutes requires
sending 32kB per infected patient. Even when compressing these EphIDs in a
cuckoo filter, we’d need around 8kB per infected patient. So smartphones would need
to download at least 2 orders of magnitude more (e.g., for 30k infected a day:
from around 1 MB to 230 MB per day).

We are working on alternatives, and explored: sending all generated EphIDs in a
cuckoo filter, using a hierarchical structure (keys per day, keys per 4 hours
etc.) so that users can do a more granular release, and smaller-region versions
of those (e.g., work per state rather than country to lower communication cost).
All of these have downsides, either in computation, bandwidth or
interoperability/leakage cost. We’re trying to find a good middle ground. If you
have an idea that we did not yet list, please do reach out to us!

### P7: Why do you call your design "decentralized" while having a backend?

We call our design decentralized because there is not central point of trust for
security and privacy. All critical operations: creating EphIDs and matching
observations are done locally in each phone. The backend server is only needed
to ensure availability. However, it does not maintain any secrets. Attackers do
not gain anything by compromising the backend. All privacy-sensitive information
is decentralized, and stored on individual’s phones.
