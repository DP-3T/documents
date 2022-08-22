# DP3T - Decentralized Privacy-Preserving Proximity Tracing

This repository documents a secure, decentralized, privacy-preserving proximity tracing system. Its goal is to simplify and accelerate the process of identifying people who have been in contact with an infected person, thus providing a technological foundation to help slow the spread of the SARS-CoV-2 virus. The system aims to minimise privacy and security risks for individuals and communities and guarantee the highest level of data protection.

# Who we are

We are a international consortium of technologists, legal experts, engineers and epidemiologists with a wide range of experience who are interested in ensuring that any proximity tracing technology does not result in governments obtaining surveillance capabilities which will endanger civil society.

The following people are behind this design:

**EPFL**: Prof. Carmela Troncoso, Prof. Mathias Payer, Prof. Jean-Pierre Hubaux, Prof. Marcel Salathé, Prof. James Larus, Prof. Edouard Bugnion, Dr. Wouter Lueks, Theresa Stadler, Dr. Apostolos Pyrgelis, Dr. Daniele Antonioli, Ludovic Barman, Sylvain Chatel 
**ETHZ**: Prof. Kenneth Paterson, Prof. Srdjan Capkun, Prof. David Basin, Dr. Jan Beutel, Dennis Jackson  
**KU Leuven**: Prof. Bart Preneel, Prof. Nigel Smart, Dr. Dave Singelee, Dr. Aysajan Abidin  
**TU Delft**: Prof. Seda Gürses  
**University College London**: Dr. Michael Veale  
**CISPA Helmholtz Center for Information Security**: Prof. Cas Cremers, Prof. Michael Backes, Dr. Nils Ole Tippenhauer  
**University of Oxford**: Dr. Reuben Binns  
**University of Torino / ISI Foundation**: Prof. Ciro Cattuto  
**Aix Marseille Univ, Université de Toulon, CNRS, CPT**: Dr. Alain Barrat  
**University of Salerno**: Prof. Giuseppe Persiano  
**IMDEA Software**: Prof. Dario Fiore  
**University of Porto (FCUP) and INESC TEC**: Prof. Manuel Barbosa  
**Stanford University**: Prof. Dan Boneh


In this repository you will find various documents defining our specification. The [white paper](DP3T%20White%20Paper.pdf) is accompanied by an [overview of the data protection aspects of the design](DP3T%20-%20Data%20Protection%20and%20Security.pdf), and a [three page simplified introduction to the protocol](DP3T%20-%20Simplified%20Three%20Page%20Brief.pdf).

In line with the [aims of the project](DP3T%20-%20Aims%20of%20the%20Project.pdf), we seek feedback from a broad audience on the high-level design, its security and privacy properties, and the functionality it offers, so that further protection mechanisms can be added to correct weaknesses. We feel it is essential that designs be made public so the community as a whole can verify the claimed privacy guarantees before applications are deployed.

Open source implementations for iOS, Android, and the back-end server are available in [other DP-3T repositories](https://github.com/DP-3T/). The DP-3T app developed for Switzerland is publicly available [Android](https://github.com/DP-3T/dp3t-app-android) and [iOS](https://github.com/DP-3T/dp3t-app-ios) and can be used as the basis for other apps

An explanatory comic [available in many languages](public_engagement/cartoon).

We publish our privacy and security analysis of specific and general proximity tracing systems. We have published a [guidebook](Security%20analysis/Privacy%20and%20Security%20Attacks%20on%20Digital%20Proximity%20Tracing%20Systems.pdf) to privacy and security risks of the entire spectrum of digital proximity tracing tools, an analysis of [PEPP-PT-NTK](Security%20analysis/PEPP-PT_%20Data%20Protection%20Architechture%20-%20Security%20and%20privacy%20analysis.pdf), and an analysis of [PEPP-PT-ROBERT](Security%20analysis/ROBERT%20-%20Security%20and%20privacy%20analysis.pdf). We have also published proposals for and an analysis of [mechanisms for upload authorisation](DP3T%20-%20Upload%20Authorisation%20Analysis%20and%20Guidelines.pdf).

In 2022, we published a retrospective analysis of the deployment of decentralised proximity tracing systems, "Deploying Decentralised, Privacy-Preserving Proximity Tracing" as Troncoso et al., 2022, available open access [here](https://dl.acm.org/doi/pdf/10.1145/3524107) and mirrored on this repository.

Contact email: [dp3t@groupes.epfl.ch](mailto:dp3t@groupes.epfl.ch).

## Joint Statement

DP-3T is listed as one of several privacy-preserving decentralized approaches to contact tracing in a joint statement from over 300 scientists from over 25 countries. The open letter is available [here](https://www.esat.kuleuven.be/cosic/sites/contact-tracing-joint-statement/).

## Apple / Google Exposure Notification

Apple and Google released a [joint specification](https://www.apple.com/newsroom/2020/04/apple-and-google-partner-on-covid-19-contact-tracing-technology/) describing their system support for a privacy-preserving exposure notification system on iOS and Android. Their proposal is very similar to our early proposal ["Low-cost decentralized proximity tracing"](https://github.com/DP-3T/documents/blob/master/DP3T%20White%20Paper.pdf).
 
DP-3T appreciates the endorsement of these two companies for our solution and has been working with both of them to implement our app on their platforms.

The Google / Apple Exposure Notification system is still evolving, in particular, the calibration of the measurement of attenuation values and duration of exposure between iOS and Android and between different phone models is still incomplete. In this phase, we have set our attenuation and duration thresholds conservatively to reduce false positives. We will evolve these thresholds as calibration improves.
 
We also strongly believe that Apple and Google should adopt our subsequent enhancements, detailed in our [white paper](https://github.com/DP-3T/documents/blob/master/DP3T%20White%20Paper.pdf), that increase user privacy. We also strongly encourage both companies to allow an external audit of their code to ensure its functionality corresponds to its specification.

## Funding

The DP3T project is not funded by Google or Apple. All of the funding project’s expenses have come from Prof. James Larus’s discretionary funds at EPFL, in anticipation of a grant from the Botnar Foundation.

Two researchers involved with the project have received funding from Google in the past. In 2019, Prof. Carmela Troncoso received a Google Security and Privacy Research Award. In 2015, Prof. Edouard Bugnion’s student received a Google PhD Fellowship. In addition, Prof. Mathias Payer received a bug bounty for finding a 0day exploit.

No participants were funded by Apple.

## April 8th, 2020: The relationship between DP-3T and PEPP-PT

*Please note that since this announcement, DP-3T partners have resigned from the PEPP-PT initiative.*

The Decentralised Privacy-Preserving Proximity Tracing (DP-3T) project is an open protocol for COVID-19 proximity tracing using Bluetooth Low Energy functionality on mobile devices that ensures personal data and computation stays entirely on an individual's phone. It was produced by a core team of over 25 scientists and academic researchers from across Europe. It has also been scrutinized and improved by the wider community.

DP-3T is a free-standing effort, originally started at EPFL and ETHZ, that has now broadened out to include stakeholders from across Europe and beyond. We develop the protocol and implement it in an open-sourced app and server on this repository.

DP-3T members have been participating in the loose umbrella of the 'Pan-European Privacy-Preserving Proximity Tracing' (PEPP-PT) project. DP-3T is not the only protocol under this umbrella. PEPP-PT also endorses centralized approaches with very different privacy properties. Pandemics do not respect borders, so there is substantial value in PEPP-PT's role of encouraging dialogue, knowledge-sharing, and interoperability.

Nevertheless, as the systems endorsed by PEPP-PT have technical differences that yield very different privacy properties, it is a mistake to use the term 'PEPP-PT' to describe a specific solution or to refer to PEPP-PT as if it embodies a single approach rather than several very different ones.
