# The Ring Notepad Application (RNote)
# Author : Mahmoud Fayed <msfclipper@yahoo.com>
# FindPrevValue() Added by Gal Zsolt 

class RNoteFind

	func OpenFindWindow
		if isobject(oSearch)
			oSearch.activatewindow()
			return
		ok
		oSearch = new qWidget()
		{
			oLblFindWhat = new qLabel(this.oSearch)
			{
				setText(T_RINGNOTEPAD_FINDWINDOWFINDWHAT) # "Find What : "
			}
			this.oSearchValue = new qlineedit(this.oSearch)
			{
				setText(this.cSearchText)
				setReturnPressedEvent(Method(:FindValue))
			}
			oLayout1 = new qHBoxLayout()
			{
				addWidget(oLblFindWhat)
				addWidget(this.oSearchValue)
			}
			oLblReplaceWith = new qLabel(this.oSearch)
			{
				setText(T_RINGNOTEPAD_FINDWINDOWREPLACEWITH) # "Replace with "
			}
			this.oReplaceValue = new qlineedit(this.oSearch)
			{
				setText(this.cReplaceText)
			}

			oLayout2 = new qHBoxLayout()
			{
				addWidget(oLblReplaceWith)
				addWidget(this.oReplaceValue)
			}
			this.oSearchCase = new qCheckbox(this.oSearch)
			{
				setText(T_RINGNOTEPAD_FINDWINDOWCASESENSITIVE) # "Case Sensitive"
			}
			oLayout3 = new qHBoxLayout()
			{
				addWidget(this.oSearchCase)
			}
			oBtnFind = new qPushButton(this.oSearch)
			{
				setText(T_RINGNOTEPAD_FINDWINDOWFINDFINDNEXT) # "Find/Find Next"
				setclickEvent(Method(:FindValue))
			}
			oBtnFindPrev = new qPushButton(this.oSearch)
			{
				setText(T_RINGNOTEPAD_FINDWINDOWFINDFINDPREV) # "Find/Find Prev"
				setclickEvent(Method(:FindPrevValue))
			}
			oBtnReplace = new qPushButton(this.oSearch)
			{
				setText(T_RINGNOTEPAD_FINDWINDOWREPLACE) # "Replace"
				setclickEvent(Method(:Replace))
			}
			oBtnReplaceAll = new qPushButton(this.oSearch)
			{
				setText(T_RINGNOTEPAD_FINDWINDOWREPLACEALL) # "Replace All"
				setclickEvent(Method(:ReplaceAll))
			}
			oBtnClose = new qPushButton(this.oSearch)
			{
				setText(T_RINGNOTEPAD_FINDWINDOWCLOSE) # "Close"
				setclickEvent(Method(:SearchClose))
			}
			oLayout4 = new qHBoxLayout()
			{
				addWidget(oBtnFind)
				addWidget(oBtnFindPrev)
				addWidget(oBtnReplace)
				addWidget(oBtnReplaceAll)
				addWidget(oBtnClose)
			}
			oLayout5 = new qVBoxLayout()
			{
				AddLayout(oLayout1)
				AddLayout(oLayout2)
				AddLayout(oLayout3)
				AddLayout(oLayout4)
			}
			setLayout(oLayout5)
			setwinicon(this.oSearch,"image/notepad.png")
			setWindowTitle(T_RINGNOTEPAD_FINDWINDOWFINDREPLACE) # "Find/Replace"
			setFixedsize(550,160)
			setwindowflags(Qt_CustomizeWindowHint | Qt_WindowTitleHint) 
			this.oSearchFilter = new qallevents(this.oSearch)
			this.oSearchFilter.setKeyPressEvent(Method(:SearchKeyPress))
			installeventfilter(this.oSearchFilter)
			setlayoutdirection(T_RINGNOTEPAD_LAYOUTDIRECTION)
			show()
		}

	func FindValue

		# Check the content and the search value
			cStr = textedit1.toplaintext()
			cValue = oSearchValue.text()
			if len(cStr) < 1 or len(cValue) < 1 return ok

		# Check the case 
			lNotCaseSensitive = ! (oSearchCase.checkState() = Qt_Unchecked)

		# Get the cursor position 
			oCursor = textedit1.textcursor()
			nPosStart = oCursor.Position()

		# Find the sub string 
			oContent = new QString2()
			oContent.append(cStr)
			nPos = oContent.indexof(cValue,nPosStart,lNotCaseSensitive)		

		# Get the substring size 
			oValue = new QString2() 
			oValue.append(cValue)
			nSize = oValue.count()

		# If we have the substring then select it 		
			if nPos >= 0
				SelectFromTo(nPos,nPos+nSize)
				return true
			ok

		return nooutput(cValue)

	func FindPrevValue

		# Check the content and the search value
			cStr = textedit1.toplaintext()
			cValue = oSearchValue.text()
			if len(cStr) < 1 or len(cValue) < 1 return ok

		# Check the case 
			lNotCaseSensitive = ! (oSearchCase.checkState() = Qt_Unchecked)

		# Get the cursor position 
			oCursor = textedit1.textcursor()
			nPosStart = oCursor.Position()
			if oCursor.HasSelection()
				nPosStart = oCursor.SelectionStart()
			ok
			nPosStart--
			nPosStart = max(nPosStart,0)

		# Find the sub string 
			oContent = new QString2()
			oContent.append(cStr)

			nPos = oContent.lastindexof(cValue,nPosStart,lNotCaseSensitive)		

		# Get the substring size 
			oValue = new QString2() 
			oValue.append(cValue)
			nSize = oValue.count()

		# If we have the substring then select it 		
			if nPos >= 0
				SelectFromTo(nPos,nPos+nSize)
				return true
			ok

		return nooutput(cValue)

	func Replace
		oCursor = textedit1.textCursor()
		if oCursor.HasSelection() = false
			SearchMessage(T_RINGNOTEPAD_FINDWINDOWREPLACE,
							T_RINGNOTEPAD_FINDWINDOWNOSELECTION)
			return false
		ok
		cValue = oSearchValue.text()
		if len(cValue) < 1 return ok
		cSelected = oCursor.SelectedText()
		if oSearchCase.checkState() = Qt_Unchecked
			cValue = lower(cValue)
			cSelected = lower(cSelected)
		ok
		if cSelected != cValue
			SearchMessage(T_RINGNOTEPAD_FINDWINDOWREPLACE,
							T_RINGNOTEPAD_FINDWINDOWNOMATCH)
			return false
		ok
		cValue = oReplaceValue.text()
		nStart = oCursor.SelectionStart()
		nEnd = oCursor.SelectionEnd()
		cStr = textedit1.toPlainText()
		cStr = left(cStr,nStart)+cValue+substr(cStr,nEnd+1)
		setTextAllowUndo(cStr)
		oCursor.setposition(nEnd+1,1)
		textedit1.settextcursor(oCursor)
		return FindValue()

	func ReplaceAll
		cStr = textedit1.toPlainText()
		cOldValue = oSearchValue.text()
		cNewValue = oReplaceValue.text()
		cnt = count(cStr,cOldValue)
		if len(cStr) < 1 or len(cOldValue) < 1 return ok
		if oSearchCase.checkState() = Qt_Unchecked
			# Not Case Sensitive
			cStr = SubStr(cStr,cOldValue,cNewValue,true)
		else
			# Case Sensitive
			cStr = SubStr(cStr,cOldValue,cNewValue)
		ok
		if cStr != textedit1.toPlainText()
			cMsg = T_RINGNOTEPAD_FINDWINDOWOPERATIONDONE # "Operation Done"
			cMsg = cMsg + T_RINGNOTEPAD_FINDWINDOWREPLACED + cnt
		else 
			cMsg = T_RINGNOTEPAD_FINDWINDOWNOTHINGTOREPLACE # "Nothing to replace!"
		ok
		setTextAllowUndo(cStr)
		SearchMessage(T_RINGNOTEPAD_FINDWINDOWREPLACEALL,cMsg)

	func setTextAllowUndo cText 
		# Get the Text Size 
			nTextSize = len(textedit1.toplaintext())
		# Select All of the Text 
			oCursor = textedit1.textcursor()
			# Save the current position 
				nPosStart = oCursor.Position()
			oCursor.setposition(0,0)
			textedit1.settextcursor(oCursor)
			oCursor = textedit1.textcursor()
			oCursor.setposition(nTextSize,1)
			textedit1.settextcursor(oCursor)
		# Set the new text using InsertPlainText() that support the Undo process 
			textedit1.InsertPlainText(cText)
		# Restore the Cursor Position 
			oCursor = textedit1.textcursor()
			oCursor.setposition(nPosStart,1)
			textedit1.settextcursor(oCursor)

	func SelectFromTo nFrom,nTo
			oCursor = textedit1.textcursor()
			oCursor.setposition(nFrom,0)
			textedit1.settextcursor(oCursor)
			oCursor = textedit1.textcursor()
			oCursor.setposition(nTo,1)
			textedit1.settextcursor(oCursor)

	func NoOutput cValue
		SearchMessage(T_RINGNOTEPAD_FINDWINDOWSEARCH,
						T_RINGNOTEPAD_FINDWINDOWCANNOTFIND + cValue)
		return false

	func SearchMessage cTitle,cContent
		new qMessagebox(oSearch)
		{
			SetWindowTitle(cTitle)
			SetText(cContent)
			show()
		}

	func Count cString,dString
		sum = 0
		while substr(cString,dString) > 0
			sum++
			cString = substr(cString,substr(cString,dString)+len(dString))
		end
		return sum

	func SearchKeyPress
		if oSearchFilter.getKeyCode() = Qt_Key_Escape
			SearchClose()
		ok

	func SearchClose
		oSearch.close()
		oSearch = NULL
		cSearchText = oSearchValue.text()
		cReplaceText = oReplaceValue.text()
