// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

import {HorseStore} from "../../src/horseStoreV1/HorseStore.sol";
import {Test, console2} from "forge-std/Test.sol";

abstract contract Base_TestV1 is Test {
    HorseStore public horseStore;

    function setUp() public virtual {
        horseStore = new HorseStore();
    }

    function testReadValue() public {
        uint256 horseCount = horseStore.readNumberOfHorses();
        assertEq(horseCount, 0);
    }

    // function testUpdateValue() public {
    //     uint256 horseNumber = 5;
    //     horseStore.updateHorseNumber(horseNumber);
    //     uint256 horseCount = horseStore.readNumberOfHorses();
    //     assertEq(horseCount, horseNumber);
    // }
}
