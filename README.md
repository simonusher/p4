
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]



<!-- PROJECT LOGO -->
<br />
<p align="center">
  <!-- <a href="https://github.com/simonusher/p4">
    <img src="images/logo.png" alt="Logo" width="80" height="80">
  </a> -->

  <h3 align="center">P4 - Parameter-less Population Pyramid for Permutations</h3>

  <p align="center">
    This project is a successful attempt at making the P3 optimization method work with scheduling problems. I created it as a part of my bachelor's thesis. Afterwards it was used in research that led to a paper published at PPSN 2020 conference. The paper is available <a href="https://link.springer.com/chapter/10.1007/978-3-030-58112-1_29">here</a>.
    <br />
    <a href="https://github.com/simonusher/p4"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/simonusher/p4/issues">Report Bug</a>
    ·
    <a href="https://github.com/simonusher/p4/issues">Request Feature</a>
  </p>
</p>



<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary><h2 style="display: inline-block">Table of Contents</h2></summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgements">Acknowledgements</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

This project contains a modified version of [Parameter-less Population Pyramid (P3)](https://dl.acm.org/doi/10.1145/2576768.2598350) by Brian W. Goldman et al. The adaptations I used were originally proposed for another linkage-learning method, namely GOMEA, by Bosman et al. in [Expanding from Discrete Cartesian to Permutation Gene-pool Optimal Mixing Evolutionary Algorithms](https://dl.acm.org/doi/10.1145/2908812.2908917).

I created it as a part of my bachelor's thesis. Afterwards it was used in research that led to a paper published at PPSN 2020 conference. The paper is available here: https://link.springer.com/chapter/10.1007/978-3-030-58112-1_29

The code includes a simple UI in Qt, but the core optimizer parts are located in src/optimizer, src/local_optimizers, and src/problem.

## Research
**This is not the exact code used in P4 paper.** If you wish to use P4 code in your research, please reach out to me or one of the other authors of the paper.

### Built With

* C++,
* [Qt](https://www.qt.io/),
* [Visual Studio 2019](https://visualstudio.microsoft.com/pl/).


<!-- LICENSE -->
## License

Distributed under the GNU GPLv3 License. See `LICENSE` for more information.



<!-- CONTACT -->
## Contact

Szymon Woźniak - swozniak6@gmail.com

Project Link: [https://github.com/simonusher/p4](https://github.com/simonusher/p4)





<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/simonusher/repo.svg?style=for-the-badge
[contributors-url]: https://github.com/simonusher/p4/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/simonusher/p4.svg?style=for-the-badge
[forks-url]: https://github.com/simonusher/p4/network/members
[stars-shield]: https://img.shields.io/github/stars/simonusher/p4.svg?style=for-the-badge
[stars-url]: https://github.com/simonusher/p4/stargazers
[issues-shield]: https://img.shields.io/github/issues/simonusher/p4.svg?style=for-the-badge
[issues-url]: https://github.com/simonusher/p4/issues
[license-shield]: https://img.shields.io/github/license/simonusher/p4.svg?style=for-the-badge
[license-url]: https://github.com/simonusher/p4/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://www.linkedin.com/in/szymon-wo%C5%BAniak-00505318a/
[p4-paper]: https://link.springer.com/chapter/10.1007/978-3-030-58112-1_29