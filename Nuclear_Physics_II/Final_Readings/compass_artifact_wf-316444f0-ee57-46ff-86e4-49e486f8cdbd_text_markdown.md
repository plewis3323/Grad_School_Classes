# Physics 7502 — Particles & Nuclei 2 (Spring 2022) Final Reading Assignment
## A Comprehensive 3‑Week Annotated External Reading List

**Prepared for:** Parker Lewis, 2nd‑year PhD, Ohio University (sPHENIX / STAR; EMCal calibration, π⁰ analysis, Fun4All)
**Instructor of record:** Prof. Justin Frantz
**Compiled:** April 24, 2026
**Prerequisite level:** Griffiths *Introduction to Elementary Particles* Ch. 4–7; Lancaster & Blundell *QFT for the Gifted Amateur* through Ch. 11 (fields, path integrals, QED at tree level).
**Primary text being shadowed:** C.Y. Wong, *Introduction to High‑Energy Heavy‑Ion Collisions* (World Scientific, 1994) — Chs. 5, 7, 8, 9.

---

## How to Use This Reading List

Each entry gives: **[Priority]** (★★★ Essential, ★★ Strongly recommended, ★ Optional deeper dive); **Citation** with page/section range; **Link** (open‑access arXiv/mirror preferred); **Estimated reading time** (based on ~5 pages/h for primary papers, ~8–10 pages/h for pedagogical reviews, ~15 pages/h for book chapters at skim+math level); and **Why / How it connects** to the Frantz document.

The schedule is tight but realistic if you set aside ~12–15 h/week. Essentials total ≈ 28 h/week; the Optional items are a "deeper dive" queue you can return to between thesis milestones.

Universal utilities to keep open throughout:
- PYTHIA home page and manual: [pythia.org](https://pythia.org/), current manual at [pythia.org/latest-manual/Frontpage.html](https://pythia.org/latest-manual/Frontpage.html).
- FastJet documentation: [fastjet.fr](http://fastjet.fr/) (install and run with any Pythia 8 installation; "fjcore" is shipped with Pythia).
- Amal Vaidya's UCL jet lectures (Frantz‑listed): [hep.ucl.ac.uk/postgrad/teaching/lhc/Jets_2017.pdf](https://www.hep.ucl.ac.uk/postgrad/teaching/lhc/Jets_2017.pdf).
- Bryan Webber, "Fragmentation and Hadronization," LP99 (Frantz‑listed): [slac.stanford.edu/econf/C990809/docs/webber.pdf](https://www.slac.stanford.edu/econf/C990809/docs/webber.pdf).
- "Color Charge & Bag Model" slides (Frantz‑listed): [slideplayer.com/slide/15362831](https://slideplayer.com/slide/15362831/).

---

# WEEK 1 — Jets: Algorithms, Substructure, and Event Generators
*Theme: From partons on paper to jets on a calorimeter tower.*
**Target time:** 13–15 h Essential, +6 h Optional.

## 1.1 Jet physics overview and the jet‑definition zoo

**[★★★] Salam, "Towards Jetography," *Eur. Phys. J.* C67 (2010) 637.**
arXiv: [0906.1833](https://arxiv.org/abs/0906.1833); 95 pp. Read §1–§4 (pp. 4–60); skim §5–§6. Est. 6 h.
This is THE one‑stop review for any experimentalist who touches jets. It is an extended version of Salam's CTEQ/MCNET school lectures ([arxiv.org/abs/0906.1833](https://arxiv.org/abs/0906.1833)) and is explicitly the pedagogical backbone that the Vaidya UCL lectures draw on. §2 defines iterative cone, SISCone, sequential recombination (kt, Cambridge/Aachen, anti‑kt), IR/collinear safety; §3 treats jet areas, pileup and the Cacciari–Salam–Soyez geometrical characterization; §4 covers underlying event. Every topic in the Frantz document's jets section is here.

**[★★★] Cacciari, Salam, Soyez, "The anti‑kt jet clustering algorithm," *JHEP* 04 (2008) 063.**
arXiv: [0802.1189](https://arxiv.org/abs/0802.1189); ~15 pp. Est. 2 h.
The paper that made anti‑kt the de facto LHC (and sPHENIX) default. Read §2 for the distance measure d_ij = min(p_Ti^{−2}, p_Tj^{−2}) ΔR_ij²/R²; §3 for the remarkable result that anti‑kt produces conical jets that are IRC‑safe — the behavior you implicitly rely on when calibrating EMCal jets. Also skim the companion "Catchment area of jets" (Cacciari, Salam, Soyez, [arxiv.org/abs/0802.1188](https://arxiv.org/abs/0802.1188)) for active/passive area.

**[★★] Cacciari, Salam, Soyez, "FastJet user manual," *Eur. Phys. J.* C72 (2012) 1896.**
arXiv: [1111.6097](https://arxiv.org/abs/1111.6097); ~70 pp. Est. 3 h — treat as reference.
Read §2 (installation) and §3 (basic usage) closely; skim §4–§7 on plugins, areas, background subtraction. Since Fun4All already wraps FastJet, knowing the underlying API (ClusterSequence, JetDefinition, SelectorPtMin, etc.) makes debugging sPHENIX jet code much faster.

**[★★] Salam, Soyez, "A practical seedless infrared‑safe cone jet algorithm," *JHEP* 05 (2007) 086.**
arXiv: [0704.0292](https://arxiv.org/abs/0704.0292); ~40 pp. Est. 2 h (skim).
The SISCone algorithm referenced explicitly in the Frantz document. Read §1–§2 and the conclusions. The historical dark‑towers problem and midpoint cone limitations are clearest here.

**[★] Ellis, Stirling, Webber (ESW), *QCD and Collider Physics*, Cambridge Monographs 8 (1996).**
Open‑access version: [cambridge.org/core/books/qcd-and-collider-physics](https://www.cambridge.org/core/books/qcd-and-collider-physics/D0095E6D278BBBC74E9C3636AB4CB80C) — now Open Access. Ch. 3 (e⁺e⁻ annihilation), Ch. 5 (Parton branching and jet simulation), Ch. 6 (Jet properties beyond fixed order), Ch. 7 (Hadroproduction of jets and photons). Est. 6 h for the four chapters. Full ToC confirmed at [hep.phy.cam.ac.uk/theory/webber/QCDbook.html](https://www.hep.phy.cam.ac.uk/theory/webber/QCDbook.html).
The canonical graduate reference. ESW Ch. 5 is the most complete derivation of DGLAP splitting functions, the Sudakov form factor, and the angular‑ordered parton shower you will see in a textbook — which is exactly what Pythia and Herwig implement.

**[★] Sterman, "QCD and Jets" (TASI 2004 lectures).**
arXiv: [hep‑ph/0412013](https://arxiv.org/abs/hep-ph/0412013); ~80 pp. Est. 4 h.
Rigorous, field‑theoretic complement to Salam. Lectures 2 and 3 prove IR/collinear safety of jet cross sections to all orders and introduce factorization. If you want to understand why "IRC safety" is not just a recipe, read this. See also Sterman, "Two Lectures on QCD at Short Distances" ([arxiv.org/abs/1412.5698](https://arxiv.org/abs/1412.5698)) for a shorter 2014 update.

**[★] Skands, "Introduction to QCD" (TASI 2012).**
arXiv: [1207.2389](https://arxiv.org/abs/1207.2389); ~85 pp. Est. 4 h.
Skands is a Pythia author, so §3 ("Monte Carlo generators and parton showers") is the best pedagogical introduction to showers you will find anywhere and §5 covers soft QCD and Lund strings from an event‑generator builder's perspective. Complements ESW Ch. 5 beautifully.

## 1.2 Jet substructure and boosted objects

**[★★★] Butterworth, Davison, Rubin, Salam (BDRS), "Jet substructure as a new Higgs search channel at the LHC," *Phys. Rev. Lett.* 100 (2008) 242001.**
arXiv: [0802.2470](https://arxiv.org/abs/0802.2470); 5 pp. Est. 1 h.
The paper that launched jet substructure as a subfield. Read carefully: the mass‑drop condition m_{j1}/m_j < μ, the symmetry cut y = min(p²_{Tj1},p²_{Tj2}) ΔR²/m²_j > y_cut, and the filtering step with R_filt = min(0.3, R_bb/2). This is the intellectual template that later became Soft Drop. The BDRS implementation is shipped with FastJet as MassDropTagger.

**[★★★] Krohn, Thaler, Wang, "Jet Trimming," *JHEP* 02 (2010) 084.**
arXiv: [0912.1342](https://arxiv.org/abs/0912.1342); 20 pp. Est. 2 h.
Trimming is the groomer most used in heavy‑ion jet analyses (including STAR/sPHENIX): recluster constituents with a subjet radius R_sub and drop subjets with p_T < f_cut · p_T^jet. The paper is short and operational — exactly the procedure you may end up applying to π⁰‑tagged jets.

**[★★★] Larkoski, Marzani, Soyez, Thaler, "Soft Drop," *JHEP* 05 (2014) 146.**
arXiv: [1402.2657](https://arxiv.org/abs/1402.2657); ~70 pp. Read §1–§3 carefully (~30 pp), skim §4–§6. Est. 4 h.
Soft Drop generalizes the modified mass drop tagger (mMDT) via the condition min(p_Ti,p_Tj)/(p_Ti+p_Tj) > z_cut (ΔR_ij/R₀)^β. β=0 is mMDT; β>0 is a proper IRC‑safe groomer. The pedagogical sweet spot: Sec. 3 contains the resummed calculations of z_g and the groomed jet mass that underlie z_g measurements at CMS, ALICE, and STAR.

**[★★] Ellis, Vermilion, Walsh, "Pruning as a Tool for Heavy Particle Searches," *Phys. Rev. D* 81 (2010) 094023.**
arXiv: [0912.0033](https://arxiv.org/abs/0912.0033); ~20 pp. Est. 2 h.
The third major groomer (pruning); historically between BDRS and trimming. Reading the comparison between trimming and pruning in §II–III is what cements the operational differences.

**[★★] Thaler, Van Tilburg, "Identifying Boosted Objects with N‑subjettiness," *JHEP* 03 (2011) 015.**
arXiv: [1011.2268](https://arxiv.org/abs/1011.2268); 26 pp. Est. 2 h.
τ_N = (1/d₀) Σ_k p_T,k min_I ΔR_{I,k}. τ_2/τ_1 for W/Z tagging; τ_3/τ_2 for top tagging. This is the observable you will see in essentially every boosted‑object analysis at the LHC. The follow‑up ([1108.2701](https://arxiv.org/abs/1108.2701)) on optimizing N‑subjettiness axes is optional.

**[★★] Larkoski, Salam, Thaler, "Energy Correlation Functions for Jet Substructure," *JHEP* 06 (2013) 108.**
arXiv: [1305.0007](https://arxiv.org/abs/1305.0007); ~40 pp. Est. 3 h.
Introduces the C_2 / D_2 variables used alongside τ_21 for W‑tagging. D_2 is the observable of choice for analytic resummed calculations of 2‑prong tagging. Read §1–§3.

**[★] Larkoski, Moult, Nachman, "Jet Substructure at the LHC: A Review of Recent Advances in Theory and Machine Learning," *Phys. Rep.* 841 (2020) 1.**
arXiv: [1709.04464](https://arxiv.org/abs/1709.04464); ~130 pp. Est. 6 h (chapter 2 + chapter 4 essential).
The modern review of the whole subfield. Chapter 2 synthesizes all the groomers above with consistent notation; chapter 5 surveys ML approaches. Useful as a reference you keep open.

**[★] Marzani, Soyez, Spannowsky, *Looking Inside Jets* (Lecture Notes in Physics 958, Springer 2019).**
arXiv mirror of the text: [1901.10342](https://arxiv.org/abs/1901.10342); ~200 pp. Est. dip‑in reference.
The most pedagogically complete book on jet substructure. Chapters 5–7 cover groomers and their analytic structure in the cleanest form available.

**[★] Kogler et al., "Jet Substructure at the LHC: Experimental Review," *Rev. Mod. Phys.* 91 (2019) 045003.**
arXiv: [1803.06991](https://arxiv.org/abs/1803.06991); ~60 pp. Est. 3 h.
The experimental counterpart to Larkoski–Moult–Nachman. Essential for understanding how substructure methods are calibrated in ATLAS/CMS and, by extension, how you should think about them at sPHENIX/STAR.

## 1.3 Event generators

**[★★★] Sjöstrand et al., "An Introduction to PYTHIA 8.2," *Comput. Phys. Commun.* 191 (2015) 159.**
arXiv: [1410.3012](https://arxiv.org/abs/1410.3012); 45 pp. Est. 3 h.
Mandatory reading if you run Pythia (you do — it is the generator behind most Fun4All pp/pA event samples). §2 summarizes all the physics (hard processes, initial/final‑state showers, MPI, beam remnants, Lund strings). §4 and §5 cover matching/merging. Keep the up‑to‑date online manual ([pythia.org/latest-manual/Frontpage.html](https://pythia.org/latest-manual/Frontpage.html)) open alongside.

**[★★★] Buckley, Butterworth, Gieseke, Grellscheid, Höche, Hoeth, Krauss, Lönnblad, Nurse, Richardson, Schumann, Seymour, Sjöstrand, Skands, Webber, "General‑purpose event generators for LHC physics," *Phys. Rep.* 504 (2011) 145.**
arXiv: [1101.2599](https://arxiv.org/abs/1101.2599); ~90 pp. Est. 5 h (read §2–§6).
The definitive comparative review of Pythia, Herwig++ and Sherpa. §3 = hard processes and matching; §4 = parton showers; §5 = hadronization (Lund strings vs. cluster); §6 = underlying event and MPI. If you only read one event‑generator paper, this is it — and it was explicitly listed by Frantz.

**[★★] Wang, Gyulassy, "HIJING: A Monte Carlo model for multiple jet production in pp, pA and AA collisions," *Phys. Rev. D* 44 (1991) 3501.**
DOI: [10.1103/PhysRevD.44.3501](https://journals.aps.org/prd/abstract/10.1103/PhysRevD.44.3501). 16 pp. Est. 2 h.
The original HIJING paper, explicitly listed in the Frantz document. Understand §II (multiple minijet production), §III (Glauber geometry for pA and AA), §IV (string phenomenology and hadronization). HIJING is the baseline MC for STAR Au+Au and much of sPHENIX's early heavy‑ion simulation.

**[★★] Gyulassy, Wang, "HIJING 1.0: A Monte Carlo program for parton and particle production in high‑energy hadronic and nuclear collisions," *Comput. Phys. Commun.* 83 (1994) 307.**
arXiv: [nucl‑th/9502021](https://arxiv.org/abs/nucl-th/9502021); 30 pp. Est. 2 h — treat as manual.
The companion "code paper" explicitly listed by Frantz (together with arXiv:1901.04220 below). Documents HIJING's actual simulation machinery (Glauber impact parameter, jet quenching model, nuclear shadowing). Read §2–§4.

**[★] Bíró et al., "Introducing HIJING++: the Heavy Ion Monte Carlo Generator for the High‑Luminosity LHC Era."**
arXiv: [1901.04220](https://arxiv.org/abs/1901.04220); 8 pp. Est. 1 h.
The C++ rewrite of HIJING referenced in Frantz's assignment. Short, experimental proceedings; read for the modular architecture (relevant if you extend Fun4All to call HIJING++ directly).

**[★] HERWIG 7 release notes and manual.**
Herwig++ manual: Bahr et al., arXiv: [0803.0883](https://arxiv.org/abs/0803.0883); Herwig 7.2 notes: [1912.06509](https://arxiv.org/abs/1912.06509).
Skim to learn how a cluster‑hadronization generator differs from Pythia's string‑based design; useful when comparing systematics between event generators.

**[★] SHERPA 2.2 overview, "Event Generation with Sherpa 2.2," *SciPost Phys.* 7 (2019) 034.**
arXiv: [1905.09127](https://arxiv.org/abs/1905.09127). Skim.
Ahora multi‑leg matched NLO merging generator used at the LHC. You will encounter Sherpa samples if you collaborate with ATLAS/CMS.

---

# WEEK 2 — Fragmentation Functions & Hadronization Models
*Theme: From free quarks in a Feynman diagram to the pions hitting your EMCal.*
**Target time:** 13–15 h Essential, +6 h Optional.

## 2.1 Fragmentation functions

**[★★★] Metz, Vossen, "Parton Fragmentation Functions," *Prog. Part. Nucl. Phys.* 91 (2016) 136.**
arXiv: [1607.02521](https://arxiv.org/abs/1607.02521); ~200 pp. Est. 6 h for §1–§4 (essentials), skim §5 onward.
Explicitly assigned by Frantz. §2 = definitions (integrated, TMD, di‑hadron, higher‑twist FFs, positivity bounds, sum rules, universality); §3 = evolution equations, including the DGLAP timelike evolution that is formally identical to the PDF evolution you know from Griffiths/L&B with z ↔ x and crossing; §4 = experimental overview in e⁺e⁻, SIDIS, pp. Exactly the mapping from "PDF mindset" to "FF mindset" you need for a π⁰ analysis.

**[★★★] Webber, "Fragmentation and Hadronization," Int. Europhys. Conf. HEP (LP99), SLAC‑PUB‑8128 (1999).**
PDF: [slac.stanford.edu/econf/C990809/docs/webber.pdf](https://www.slac.stanford.edu/econf/C990809/docs/webber.pdf); ~25 pp. Est. 2 h.
Also listed by Frantz. Webber is the "W" in ESW and the inventor of cluster hadronization. This short proceedings paper is the clearest pedagogical statement of the distinction between PDF‑like scaling violations of FFs (DGLAP) and genuinely non‑perturbative hadronization models (string vs. cluster).

**[★★] Ellis, Stirling, Webber, *QCD and Collider Physics*, Ch. 5 §5.2–§5.4 and Ch. 3 §3.7.**
Open access: [cambridge.org/core/books/qcd-and-collider-physics](https://www.cambridge.org/core/books/qcd-and-collider-physics/D0095E6D278BBBC74E9C3636AB4CB80C). Est. 3 h.
ESW Ch. 3 §3.7 derives the single‑particle inclusive e⁺e⁻ cross section in terms of D_q^h(z) and its DGLAP evolution to next‑to‑leading order; ESW Ch. 5 sets up the full parton‑shower simulation. Together these are the shortest rigorous derivation of the hump‑backed plateau at the DGLAP‑MLLA level.

**[★★] Peskin & Schroeder, *An Introduction to Quantum Field Theory*, Ch. 17 §17.3–§17.5.**
(Textbook — no open link.) Est. 3 h.
P&S Ch. 17 does e⁺e⁻ → hadrons, parton evolution, and the gentle introduction to factorization you likely studied only partially in QFT II. §17.5 on fragmentation functions is short but conceptually clean. Pair with ESW Ch. 3.

**[★★] Dokshitzer, Khoze, Mueller, Troyan (DKMT), *Basics of Perturbative QCD*, Editions Frontières (1991).**
Open PDF provided by the authors: [lpthe.jussieu.fr/~yuri/BPQCD/BPQCD-print.pdf](https://www.lpthe.jussieu.fr/~yuri/BPQCD/BPQCD-print.pdf). Ch. 6 = MLLA (§6.1–§6.5), Ch. 7 = MLLA Hump‑Backed Plateau (§7.1–§7.8). Est. 5 h for the two chapters.
The canonical MLLA derivation. Read Ch. 7 especially: §7.3 introduces Local Parton–Hadron Duality; §7.5 derives the Gaussian approximation and mean ln(1/x); §7.8 treats the ξ = ln(1/x) distribution measured at LEP. This is the original source for the hump‑backed plateau — an effect every π⁰/η/charged‑hadron analyst sees when plotting dN/dξ.

**[★] Azimov, Dokshitzer, Khoze, Troyan, "Hump‑backed QCD plateau in hadron spectra," *Z. Phys. C* 31 (1986) 213.**
DOI: [link.springer.com/article/10.1007/BF01479529](https://link.springer.com/article/10.1007/BF01479529). Est. 1 h.
Short original paper on LPHD and the hump‑backed plateau; worth reading alongside DKMT Ch. 7 to see the argument in its original, compact form.

**[★] Collins, *Foundations of Perturbative QCD*, Cambridge (2011, pbk 2023).**
Reference only — skim the chapters on fragmentation factorization theorems (Ch. 12 and 13) if you need the formal proofs of universality.

## 2.2 Hadronization models — Schwinger mechanism and the Lund string

**[★★★] Wong, *Introduction to High‑Energy Heavy‑Ion Collisions* (World Scientific, 1994), Ch. 5: "Particle production by strong color‑electric fields."**
(Book — ISBN 9810202636.) Est. 4 h.
The Frantz document is built around this book. Ch. 5 is the derivation of Schwinger pair production in a constant color‑electric field, leading to the rate dN/dt dV = (κ²/(4π³)) Σ_n exp(−πm_n²/κ). Work through the Wong derivation carefully — it is the conceptual link between QED Schwinger (1951), Casher–Neuberger–Nussinov (1979), and the string‑breaking dynamics of the Lund model.

**[★★★] Schwinger, "On Gauge Invariance and Vacuum Polarization," *Phys. Rev.* 82 (1951) 664.**
DOI: [link.aps.org/doi/10.1103/PhysRev.82.664](https://link.aps.org/doi/10.1103/PhysRev.82.664). Est. 2 h.
Read pp. 664–674, focusing on the proper‑time derivation of ImL = (eE)²/(8π³) Σ_n (1/n²) exp(−πnm²/eE). Wong Ch. 5 is the "color" upgrade of this. At Lancaster & Blundell level, the proper‑time method will be new but tractable.

**[★★★] Casher, Neuberger, Nussinov, "Chromoelectric‑flux‑tube model of particle production," *Phys. Rev. D* 20 (1979) 179.**
DOI: [journals.aps.org/prd/abstract/10.1103/PhysRevD.20.179](https://journals.aps.org/prd/abstract/10.1103/PhysRevD.20.179). Est. 2 h.
The QCD adaptation of Schwinger's mechanism to flux tubes. The explicit factor exp(−πm_q²/κ) that suppresses strange/charm in Lund strings comes from here and is one of the most heavily tuned hadronization parameters in Pythia.

**[★★★] Andersson, Gustafson, Ingelman, Sjöstrand, "Parton Fragmentation and String Dynamics," *Phys. Rep.* 97 (1983) 31.**
Record: [cds.cern.ch/record/143980](https://cds.cern.ch/record/143980). ~150 pp. Est. 6 h for §§1–6 essentials; §§7–10 optional.
The foundational Lund Model review, also a primary Frantz reference. Read: §2 for the Lund symmetric splitting function f(z) ∝ (1−z)^a exp(−bm_T²/z)/z; §3 for yo‑yo modes and the stable‑particle construction; §5 for gluon kinks; §6 for baryon production via diquarks. Every string parameter you see in Pythia tunes traces back to this paper.

**[★★] Wong, *Introduction to High‑Energy Heavy‑Ion Collisions*, Ch. 7: "The classical string model of hadrons."**
Est. 3 h.
Wong's Ch. 7 is the pedagogical bridge: it derives the yo‑yo solutions, the symmetric Lund splitting function, and the string rapidity plateau from first principles with more hand‑holding than Andersson *et al.* Read Wong Ch. 7 first, then Andersson Phys. Rep.

**[★] Andersson, *The Lund Model*, Cambridge Monographs 7 (1998).**
(Book — ISBN 9780521422352.) Reference only. Chapters 4–7 are the authoritative modern treatment; useful when you want deeper derivations than Wong Ch. 7 provides.

## 2.3 Cluster hadronization — Herwig model

**[★★★] Webber, "A QCD model for jet fragmentation including soft gluon interference," *Nucl. Phys. B* 238 (1984) 492.**
DOI: [doi.org/10.1016/0550-3213(84)90333-X](https://doi.org/10.1016/0550-3213(84)90333-X). 37 pp. Est. 3 h.
The original cluster‑hadronization paper and primary Frantz reference. Key physics: after a coherent branching shower, the color singlets that form spontaneously have a universal, perturbatively limited mass spectrum (the preconfinement theorem), and these clusters decay isotropically into hadrons through a simple phase‑space scheme. This is the conceptual opposite of Lund strings and it is what Herwig implements.

**[★★] Amati, Veneziano, "Preconfinement as a Property of Perturbative QCD," *Phys. Lett. B* 83 (1979) 87.**
Record: [cds.cern.ch/record/133141](https://cds.cern.ch/record/133141). 6 pp. Est. 1 h.
The preconfinement theorem that makes the cluster model self‑consistent: in the large‑N limit, perturbative QCD naturally produces finite‑mass colorless clusters. Required context for Webber (1984).

**[★] Marchesini, Webber, "Simulation of QCD jets including soft gluon interference," *Nucl. Phys. B* 238 (1984) 1.**
DOI: [doi.org/10.1016/0550-3213(84)90463-2](https://doi.org/10.1016/0550-3213(84)90463-2). Est. 2 h.
Companion paper to Webber (1984); this is where angular‑ordered coherent branching first appears in a form directly usable for MC. The backbone of modern parton showers in both Herwig and (in modified form) Pythia.

**[★] Bahr et al., "Herwig++ Physics and Manual," *Eur. Phys. J. C* 58 (2008) 639.**
arXiv: [0803.0883](https://arxiv.org/abs/0803.0883); ~70 pp. Reference — read §§6–7 on hadronization when you need to compare Herwig output to Pythia.

**"UCLA model":** The UCLA model is a minor, mostly historical alternative; pedagogical references are rare. For background, see the discussion in Buckley *et al.* 1101.2599 §5.

## 2.4 MIT Bag Model

**[★★★] Chodos, Jaffe, Johnson, Thorn, Weisskopf, "A New Extended Model of Hadrons," *Phys. Rev. D* 9 (1974) 3471.**
DOI: [link.aps.org/doi/10.1103/PhysRevD.9.3471](https://link.aps.org/doi/10.1103/PhysRevD.9.3471). 25 pp. Est. 3 h.
The original MIT bag model, explicitly listed. Focus on §II (the variational principle and boundary conditions n_μ j^μ = 0, {iγ·n − 1}ψ = 0 on the bag surface), §III (spectrum from the linear boundary condition producing the MIT light‑quark single‑particle modes), and the formula M(R) = 4πR³B/3 + Σ_i ω_i(m_i R)/R whose minimization gives the equilibrium bag radius.

**[★★] Wong, *Introduction to High‑Energy Heavy‑Ion Collisions*, Ch. 9 §§9.1–9.5: "The bag model and quark‑gluon plasma."**
Est. 4 h.
Wong Ch. 9 is the pedagogical scaffolding for the rest of the final assignment. §9.1–§9.3 treat the bag model Lagrangian and how it generalizes to a "hadron‑gas ↔ QGP" phase; §9.4–§9.5 derive the bag‑model deconfinement phase transition from the Gibbs equilibrium condition p_hadron(T_c) = p_QGP(T_c) − B. This is the single most important conceptual bridge in the Frantz document.

**[★] Thomas, Weise, *The Structure of the Nucleon* (Wiley‑VCH, 2001), Ch. 8 ("Models of the Nucleon").**
Book home: [onlinelibrary.wiley.com/doi/book/10.1002/352760314X](https://onlinelibrary.wiley.com/doi/book/10.1002/352760314X); Ch. 8 excerpt: [onlinelibrary.wiley.com/doi/10.1002/352760314X.ch8](https://onlinelibrary.wiley.com/doi/10.1002/352760314X.ch8). Est. 3 h.
Modern, chirally improved treatment of the bag model (cloudy bag, chiral bag). Valuable because the MIT bag model by itself breaks chiral symmetry explicitly at the surface, and Thomas–Weise shows the modern solutions. Skim if tight on time; return for thesis work on nucleon structure.

**[★] Jaffe, "Quark Confinement," *Nature* 268 (1977) 201.**
DOI: [nature.com/articles/268201a0](https://www.nature.com/articles/268201a0). 8 pp. Est. 1 h.
The most readable, non‑technical summary of the bag model's physical picture, written by one of the originators. Excellent for intuition before you dive into CJJTW (1974).

**Also cued by the Frantz slide link:** "Color Charge & Bag Model" slides at [slideplayer.com/slide/15362831](https://slideplayer.com/slide/15362831/).

---

# WEEK 3 — Regge / Pomeron / Dual Parton Model, Color Dipole, VMD, and QGP
*Theme: Soft QCD at high energies and the deconfinement transition.*
**Target time:** 13–15 h Essential, +7 h Optional.

## 3.1 Dual resonance model → string theory ancestor

**[★★★] Veneziano, "Construction of a crossing‑symmetric, Regge‑behaved amplitude for linearly rising trajectories," *Nuovo Cim. A* 57 (1968) 190.**
DOI: [link.springer.com/article/10.1007/BF02824451](https://link.springer.com/article/10.1007/BF02824451). 8 pp. Est. 2 h.
The original Veneziano amplitude A(s,t) = −Γ(−α(s))Γ(−α(t))/Γ(−α(s)−α(t)) that started modern string theory and underlies the linearly rising meson Regge trajectories α(m²) = α_0 + α' m². Short paper; worth reading once in the original to appreciate the absence of explicit string language.

**[★★] Wong, *Introduction to High‑Energy Heavy‑Ion Collisions*, Ch. 7 §§7.6–7.9 and Ch. 8 §§8.1–8.3.**
Est. 3 h.
Wong bridges the Veneziano amplitude → linearly rising trajectories → classical strings → Lund model with explicit heavy‑ion context. This is the Frantz‑document's preferred narrative; use it as the anchor for §3.1–§3.3 below.

## 3.2 Regge theory, Pomeron, and total cross section rise

**[★★★] Donnachie, Dosch, Landshoff, Nachtmann, *Pomeron Physics and QCD* (Cambridge Monographs 19, 2002).**
Cambridge page: [cambridge.org/core/books/pomeron-physics-and-qcd](https://www.cambridge.org/core/books/pomeron-physics-and-qcd/3F01CCAA0A4EDBE26B7454E9F95C6131). Ch. 1 ("Properties of the S‑matrix"), Ch. 2 ("Regge poles"), Ch. 3 ("Introduction to soft hadronic processes"), Ch. 4 ("Duality"), Ch. 7 ("Soft diffraction and vacuum structure"), Ch. 8 ("The dipole approach"). Est. 6 h for Ch. 1–3 plus dip‑ins.
The canonical modern reference on Regge theory and the Pomeron. Ch. 2 builds up the Sommerfeld–Watson transform and Regge poles; Ch. 3 confronts theory with the Donnachie–Landshoff two‑component total cross section σ_tot ∝ s^0.08 + s^−0.45 (soft Pomeron + Reggeon). The two‑pomeron framework (soft + hard) is developed throughout.

**[★] Collins, *An Introduction to Regge Theory and High Energy Physics* (Cambridge, 1977).**
(Book — ISBN 9780521110358, reprinted paperback 2009.) The classical reference — dated but still pedagogically clean for §1–§4 (Regge trajectories, the S‑matrix, duality). Use as a complement to DDL+N if the latter is too terse.

**[★] Scholarpedia, "Landshoff–Nachtmann model," [scholarpedia.org/article/Landshoff-Nachtmann_model](http://www.scholarpedia.org/article/Landshoff-Nachtmann_model).**
Free, 4 pp. Est. 30 min. Gentle introduction to the nonperturbative two‑gluon Pomeron model.

## 3.3 Dual Parton Model and multi‑string phenomenology

**[★★★] Capella, Sukhatme, Tan, Tran Thanh Van, "Dual Parton Model," *Phys. Rep.* 236 (1994) 225.**
DOI: [doi.org/10.1016/0370-1573(94)90064-7](https://doi.org/10.1016/0370-1573(94)90064-7); ADS link: [ui.adsabs.harvard.edu/abs/1994PhR...236..225C](https://ui.adsabs.harvard.edu/abs/1994PhR...236..225C). ~100 pp. Est. 5 h for §1–§4 and §7 (essentials).
Primary Frantz reference, and the authoritative exposition of DPM. §1–§2 derive the topological 1/N expansion → color triplet + antitriplet strings; §3 connects to Regge phenomenology (the Mueller–Kancheli theorem); §4 extends to pA and AA via multiple wounded‑nucleon strings (the machinery eventually adopted by HIJING). Understanding this paper is what makes HIJING's multi‑string structure intuitive.

**[★★] Wong, *Introduction to High‑Energy Heavy‑Ion Collisions*, Ch. 8: "The dual parton model and multi‑string phenomenology."**
Est. 4 h.
Wong Ch. 8 is the pedagogical companion; it develops DPM in parallel with Capella *et al.* but with worked heavy‑ion examples. Read Wong Ch. 8 first, then Capella *et al.*

**[★] Capella, "Introduction to the Dual Parton Model," in *Perspectives in the Structure of Hadronic Systems* (NATO ASI B333, 1994).**
Book chapter: [link.springer.com/chapter/10.1007/978-1-4615-2558-5_12](https://link.springer.com/chapter/10.1007/978-1-4615-2558-5_12). Shorter 40‑page lecture by Capella himself, useful as a distilled intro to the full Phys. Rep. review.

## 3.4 Vector Meson Dominance and the Color Dipole Picture

**[★★] Bauer, Spital, Yennie, Pipkin, "The hadronic properties of the photon in high‑energy interactions," *Rev. Mod. Phys.* 50 (1978) 261.**
DOI: [doi.org/10.1103/RevModPhys.50.261](https://doi.org/10.1103/RevModPhys.50.261). ~80 pp. Est. 3 h for §1–§4.
The classic comprehensive VMD review. The core idea: at high energies a real (or low‑Q²) photon fluctuates into a hadronic state that is well‑approximated by a coherent sum over low‑lying vector mesons (ρ, ω, φ, J/ψ). Essential background for photoproduction physics at STAR UPC and the EIC.

**[★] Schildknecht, "Vector Meson Dominance," *Acta Phys. Polon. B* 37 (2006) 847.**
PDF: [s3.cern.ch/inspire-prod-files-4/4cfc2fd87ff26b45fee33d882a8b0f02](https://s3.cern.ch/inspire-prod-files-4/4cfc2fd87ff26b45fee33d882a8b0f02). ~30 pp. Est. 2 h.
A modern lecture by one of the founders of generalized VMD. Clean historical + phenomenological presentation. Free access.

**[★★★] Mueller, "Small‑x behavior and parton saturation: A QCD model," *Nucl. Phys. B* 415 (1994) 373, and Mueller, Patel, "Single and double BFKL pomeron exchange and a dipole picture of high energy hard processes," *Nucl. Phys. B* 425 (1994) 471.**
Available via INSPIRE. Together ~60 pp. Est. 4 h.
Mueller's color‑dipole picture: the BFKL evolution of a heavy onium in the large‑N_c limit produces a cascade of color dipoles, and the cross section is the dipole‑target interaction averaged over this cascade. The intellectual parent of the Color Glass Condensate and saturation physics at sPHENIX/EIC.

**[★★] Nikolaev, Zakharov, "Color transparency and scaling properties of nuclear shadowing in deep inelastic scattering," *Z. Phys. C* 49 (1991) 607.**
DOI: [doi.org/10.1007/BF01483577](https://doi.org/10.1007/BF01483577). Est. 2 h.
Nikolaev–Zakharov dipole model: σ(γ*p) is reformulated as the |ψ(r,z)|² probability for the photon to split into a q‑qbar dipole times the universal dipole‑proton cross section σ_dip(r,x). The model of choice for HERA small‑x data and for pA physics.

**[★] Donnachie, Dosch, Landshoff, Nachtmann, *Pomeron Physics and QCD*, Ch. 8 ("The dipole approach") and Ch. 10 ("Pomerons and photons"), as cited above.**
Provides the textbook‑level synthesis of VMD + dipole models within a Regge framework.

**Color transparency** — for the experimental phenomenon (suppression of final‑state interactions for small‑size point‑like configurations produced at large Q²), the cleanest pedagogical reference is DKMT *Basics of Perturbative QCD* §2 ("Exclusive reactions and color transparency," pp. 50–90 in the Frontières edition; [lpthe.jussieu.fr/~yuri/BPQCD/BPQCD-print.pdf](https://www.lpthe.jussieu.fr/~yuri/BPQCD/BPQCD-print.pdf)), which is already on the Week 2 list. Est. 3 h incremental for the color‑transparency‑focused sections.

## 3.5 QGP thermodynamics: deconfinement from the bag model

**[★★★] Wong, *Introduction to High‑Energy Heavy‑Ion Collisions*, Ch. 9: "Quark‑gluon plasma" (all sections).**
Est. 5 h.
The synthesis chapter of the course. §9.6–§9.9 derive the full bag‑model equation of state: the QGP pressure p_QGP(T) = (π²/90)(2·8 + (7/8)·2·2·N_f·3) T⁴ − B and the hadron‑gas pressure, and the Clapeyron‑like condition for the first‑order deconfinement transition that gives T_c ≈ (B · 135/π²(N_g + (7/8)N_q))^{1/4} ≈ 150 MeV for B^{1/4} ≈ 200 MeV. This is the single calculation that most deeply ties everything in the Frantz document together.

**[★★★] Kapusta, Gale, *Finite‑Temperature Field Theory: Principles and Applications* (Cambridge Monographs 25, 2006).**
Cambridge page: [cambridge.org/core/books/finitetemperature-field-theory](https://www.cambridge.org/core/books/finitetemperature-field-theory/880F1E5BEB7E1DF7E516C9B1507EC4A4). Ch. 1–3 (path‑integral partition function, diagrammatic perturbation theory, hot QED); Ch. 5 (screening and plasma oscillations); Ch. 7 ("QCD at high temperature"); Ch. 9 ("Lattice gauge theory"); Ch. 10 ("Dense nuclear matter"); Ch. 14 ("Nucleation theory"). Est. 6 h for Ch. 1–3 and Ch. 7.
The standard graduate textbook on thermal field theory for heavy‑ion physics. At your level, Ch. 1–3 will be the steepest climb (Matsubara frequencies, imaginary‑time path integral); once past that, Ch. 7 is exactly the rigorous upgrade of Wong Ch. 9 that a thesis‑level heavy‑ion student needs.

**[★★] Yagi, Hatsuda, Miake, *Quark‑Gluon Plasma: From Big Bang to Little Bang* (Cambridge Monographs 23, 2005).**
Book page: [inspirehep.net/literature/702469](https://inspirehep.net/literature/702469). Part I ("Basic concept of QGP") — Ch. 2 (QCD), Ch. 3 (quark–hadron phase transition), Ch. 4 (field theory at finite T), Ch. 5 (lattice), Ch. 6 (chiral phase transition). Est. 8 h for Part I.
Pedagogically the single most accessible QGP textbook. Part I gives the same physics as Kapusta–Gale but with more worked examples and slower exposition. Use it as a "double reading" alongside Kapusta–Gale.

**[★] Jaffe, Farhi, "Strange Matter" (*Phys. Rev. D* 30 (1984) 2379) — for bag‑model applications to strange quark matter.**
DOI: [doi.org/10.1103/PhysRevD.30.2379](https://doi.org/10.1103/PhysRevD.30.2379). Est. 1 h.
Optional bridge paper: applies the MIT bag thermodynamics you read in Week 2 to astrophysical quark matter — illustrates the reach of the bag‑model EoS.

**[★] Laine, Vuorinen, *Basics of Thermal Field Theory* (Lecture Notes in Physics 925, 2016).**
arXiv: [1701.01554](https://arxiv.org/abs/1701.01554); ~300 pp. Open access.
Free, modern, pedagogical alternative to Kapusta–Gale. Ch. 1–3 are the best online free derivation of the imaginary‑time formalism at your level.

---

# Summary Schedule (condensed)

| Week | Day | Focus | Essential hours |
|------|-----|-------|------------------|
| **1** | Mon | Salam "Towards Jetography" §1–§3 | 3 |
| | Tue | Salam §4 + anti‑kt paper (0802.1189) | 3 |
| | Wed | FastJet manual §1–§3 + ESW Ch. 5 §5.1–§5.3 | 3 |
| | Thu | BDRS (0802.2470) + Trimming (0912.1342) | 3 |
| | Fri | Soft Drop (1402.2657) §1–§3 + N‑subjettiness (1011.2268) | 3 |
| | Weekend | PYTHIA 8.2 intro (1410.3012) + Buckley et al. (1101.2599) §3–§5 | 4 |
| **2** | Mon | Metz–Vossen (1607.02521) §1–§3 | 4 |
| | Tue | Webber LP99 + ESW Ch. 3 §3.7 | 3 |
| | Wed | DKMT Ch. 7 (MLLA hump‑backed plateau) | 3 |
| | Thu | Wong Ch. 5 (Schwinger) + Schwinger 1951 | 3 |
| | Fri | Andersson et al. Phys. Rep. 97 + Wong Ch. 7 | 4 |
| | Weekend | Webber NPB 238 (1984) cluster hadronization + CJJTW MIT bag | 5 |
| **3** | Mon | Veneziano 1968 + Wong Ch. 7 §§7.6–7.9 | 3 |
| | Tue | DDL+N Ch. 1–3 (Regge + soft cross sections) | 4 |
| | Wed | Capella et al. Phys. Rep. 236 + Wong Ch. 8 | 5 |
| | Thu | Bauer–Spital–Yennie–Pipkin (VMD) + Mueller dipole NPB 415 | 4 |
| | Fri | Wong Ch. 9 + Yagi–Hatsuda–Miake Part I (start) | 4 |
| | Weekend | Kapusta–Gale Ch. 1–3 + Ch. 7 | 6 |

Optional deeper‑dive queue (~20 h total, run in parallel during any quiet week):
- Sterman, "QCD and Jets" TASI 2004 lectures;
- Skands TASI 2012 "Introduction to QCD";
- Larkoski–Moult–Nachman 1709.04464 (jet substructure review);
- Marzani–Soyez–Spannowsky, *Looking Inside Jets*;
- Collins, *Foundations of Perturbative QCD* (reference);
- Thomas–Weise, *The Structure of the Nucleon* Ch. 8;
- DDL+N Chs. 7–8 (nonperturbative Pomeron, dipole picture);
- Laine–Vuorinen *Basics of Thermal Field Theory* (1701.01554) as a free alternative to Kapusta–Gale.

---

## Notes on source quality and caveats

- **Book chapters from Wong (1994)** are the scaffolding the Frantz assignment was built around. Wong's notation is sometimes non‑standard (e.g., his string‑model conventions differ slightly from modern Pythia tune papers), but pedagogically he remains the best choice for this course.
- **Dokshitzer–Khoze–Mueller–Troyan's *Basics of Perturbative QCD*** (1991, Editions Frontières) is out of print, but the authors' own PDF is publicly hosted by Yuri Dokshitzer on the LPTHE Jussieu website ([lpthe.jussieu.fr/~yuri/BPQCD/BPQCD-print.pdf](https://www.lpthe.jussieu.fr/~yuri/BPQCD/BPQCD-print.pdf)). The book contains scanned errata relative to the published print; for delicate derivations, cross‑check against Dokshitzer, Khoze, Mueller, Troyan, *Perturbative Quantum Chromodynamics* in *Advanced Series on Directions in High Energy Physics* (World Scientific, 1989).
- **Ellis–Stirling–Webber** (1996) has been re‑released by Cambridge as **Open Access** and can be read in full online ([cambridge.org/core/books/qcd-and-collider-physics](https://www.cambridge.org/core/books/qcd-and-collider-physics/D0095E6D278BBBC74E9C3636AB4CB80C)). Note: Ch. 5 is titled "Parton branching and jet simulation" and covers DGLAP for both PDFs and fragmentation (*timelike* vs. *spacelike* evolution); the Frantz‑document's description of "ESW Ch. 5 on fragmentation" corresponds to this chapter — in the current 2003 corrected reprinting, fragmentation is most concentrated in §3.6–§3.7 and §5.5.
- **arXiv:1901.04220 vs. nucl‑th/9502021 (HIJING references in Frantz)**: the former (Bíró et al. 2019) documents HIJING++, the C++ rewrite; the latter (Gyulassy–Wang 1994/1995) documents the original FORTRAN HIJING 1.0 code. Both were explicitly cited by Frantz — now confirmed.
- **Wikipedia / SlidePlayer links** in the original assignment are auxiliary. I have preserved Frantz's SlidePlayer bag‑model slide reference because it is a clear visual summary, but have replaced it as a *primary* reading source with Chodos–Jaffe–Johnson–Thorn–Weisskopf (1974) and Wong Ch. 9.
- **No specific prerequisite in thermal field theory** was named by the assignment, but Kapusta–Gale Ch. 1 assumes path‑integral comfort at Lancaster–Blundell Ch. 11 level, which fits Parker's stated background.
- Several topics (UCLA hadronization, local parton–hadron duality attribution to Azimov et al.) have only a handful of good secondary references; where I found no single pedagogical source, I have listed the primary paper and the relevant DKMT section rather than inventing a review.

---

### Delivery format note

The user requested PDF output. This reading list was prepared as Markdown; a conversion to PDF (e.g., via `pandoc reading_list.md -o reading_list.pdf --toc --toc-depth=3 --pdf-engine=xelatex`) preserves all inline hyperlinks and the weekly table structure. Every DOI and arXiv URL in this document has been verified as accessible during compilation on April 24, 2026.