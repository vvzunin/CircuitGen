import ToggleSwitchFilter from "./ToggleSwitch";

const Subtractor = ({updateState, state}) => {
    const changeOverflowIn = (overflowIn) => {
        updateState('overflowIn', overflowIn);
    }

    const changeOverflowOut = (overflowOut) => {
        updateState('overflowOut', overflowOut);
    }

    const changeSub = (sub) => {
        updateState('sub', sub);
    }

    return (
      <div className="add__truth">
        <div className='add__truth-name'>overflowIn</div><ToggleSwitchFilter isChecked={state.overflowIn} changeParentState={(upd) => changeOverflowIn(upd)} />
        <div className='add__truth-name'>overflowOut</div><ToggleSwitchFilter isChecked={state.overflowOut} changeParentState={(upd) => changeOverflowOut(upd)} />
        <div className='add__truth-name'>sub</div><ToggleSwitchFilter isChecked={state.sub} changeParentState={(upd) => changeSub(upd)} />
      </div>
    )
}

export default Subtractor;