import React from 'react'
import { useId } from "react-id-generator";


const ToggleSwitchFilter = ({changeParentState, isChecked = false, name = ""}) => {
	const [htmlId] = useId();	

	function changeCheckbox() {
	   {changeParentState && changeParentState(!isChecked);}
	}

	return (
	<div className="toggleSwitch">
		<input id={name == "" ? htmlId : name} type="checkbox" checked={isChecked} onChange={changeCheckbox}/>
        <label htmlFor={name == "" ? htmlId : name} className={isChecked ? "active" : ""}>Toggle</label>
	</div>
	)
}

export default ToggleSwitchFilter;