import ToggleSwitchFilter from "./ToggleSwitch";

const Comparsion = ({updateState, state}) => {
    const changeEqual = (equal) => {
        updateState('equal', equal);
    }

    const changeLess = (less) => {
        updateState('less', less);
    }

    const changeMore = (more) => {
        updateState('more', more);
    }

    return (
      <div className="add__truth">
        <div className='add__truth-name'>equal</div><ToggleSwitchFilter isChecked={state.equal} changeParentState={(upd) => changeEqual(upd)} />
        <div className='add__truth-name'>less</div><ToggleSwitchFilter isChecked={state.less} changeParentState={(upd) => changeLess(upd)} />
        <div className='add__truth-name'>more</div><ToggleSwitchFilter isChecked={state.more} changeParentState={(upd) => changeMore(upd)} />
      </div>
    )
}

export default Comparsion;