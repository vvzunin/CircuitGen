import ToggleSwitchFilter from "./ToggleSwitch";

const Summator = ({updateState, state}) => {
    const changeOverflowIn = (overflowIn) => {
        updateState('overflowIn', overflowIn);
    }

    const changeOverflowOut = (overflowOut) => {
        updateState('overflowOut', overflowOut);
    }

    const changeMinus = (minus) => {
        updateState('minus', minus);
    }

    return (
      <div className="add__truth">
        <div className='add__truth-name'>overflowIn</div><ToggleSwitchFilter isChecked={state.overflowIn} changeParentState={(upd) => changeOverflowIn(upd)}/>
        <div className='add__truth-name'>overflowOut</div><ToggleSwitchFilter isChecked={state.overflowOut} changeParentState={(upd) => changeOverflowOut(upd)}/>
        <div className='add__truth-name'>minus</div><ToggleSwitchFilter isChecked={state.minus} changeParentState={(upd) => changeMinus(upd)}/>
      </div>
    )
}

export default Summator;