# Backoffice process for disclosure of contacts

# Assumptions

*	Done by a medical professional in the context of a normal consult.

*	Thus allowing for a professional assessment

*	Thus allowing for accurate and appropriate advice, followups and so on -- meeting general accepted standards of medical care.

*	Thus ensuring that the right (medical) information is captures in the right (separate) systems.

*	Thus ensuring that the patient is able to provide an informed consent. With information professionally presented by a licensed medical professional at the appropriate levels and in the context of a treatment, patient/doctor relation.

# Process

*	At some point, after explaining the ramifications the patient provides his or her informed consent to share the opaque infection information

*	The medical professional may, or may not, log this as is required by their professional standards.

*	The patient accesses a special section in the App and shows or conveys (e.g. over the phone) a sequence of (cryptographically random) 6 + 4 numbers or a sequence of words (aka as trustwords or ``car battery horse staple') in an appropriate language.

*	The app posts a 32 byte hash of the opaque data that it intends to submit to a backend service with the first 6 digits as a reference.

*	The health professional enters the the 6 + 4 digits into a backoffice systems; the system locates the hash an the professional then  his or her UZI chipcard to digitally sign the hash+6+4 digits and posts the resulting S/MIME package back to the backend.

*	The phone of the user picks this the signed package up; validates the signature of the professional using a build in root certifcate, paying attention to the 'flags' in the professional certficate (e.g. Zorgverlener waarvan het beroep valt onder artikel 3 van de Wet BIG) and if correct - posts the relevant opaque seeds to the backend along with the S/MIME package that contains the signed hash/10 digits. It then destroys any seeds and rekeys (as per the protocol).

*	The backend receives this data, verifies the S/MIME signature, verifies that the hash contained in it matches that of the submitted seed and queues the seeds up to the next aggregation cycle.

	

